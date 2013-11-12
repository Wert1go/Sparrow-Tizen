/*
 * ImageRequestOperation.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#include "ImageRequestOperation.h"
#include "RestClient.h"

#include <FNet.h>
#include <FWeb.h>
#include <FGraphics.h>
#include <FMedia.h>
#include "ImageCache.h"

#include "dispatch/dispatch.h"

using namespace Tizen::Base;
using namespace Tizen::Net::Http;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

ImageRequestOperation::ImageRequestOperation(const Tizen::Base::String *url) {

	HttpHeader* pHeader = null;
	__pUrl = new String(url->GetPointer());
	__pHttpTransaction = RestClient::getInstance().GetActiveSession()->OpenTransactionN();
	__pHttpTransaction->AddHttpTransactionListener(*this);

	HttpRequest* pHttpRequest = __pHttpTransaction->GetRequest();

	pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	pHttpRequest->SetUri(url->GetPointer());
	pHeader = pHttpRequest->GetHeader();
	pHeader->AddField(L"Accept", L"image/*");

	__pRequestOwner = null;
	__pByteBuffer = null;
	__isComplited = false;
}

ImageRequestOperation::~ImageRequestOperation() {
	AppLogDebug("ImageRequestOperation::~ImageRequestOperatio");
	delete __pHttpTransaction;
	__pHttpTransaction = null;
	delete __pUrl;
	__pUrl = null;
	delete __pByteBuffer;
}

void ImageRequestOperation::perform() {
	if (__pHttpTransaction != null) {
		__pHttpTransaction->Submit();
	} else {
		AppLogDebug("ImageRequestOperation::Ошибка при попытке выполнить HTTP запрос");
	}
}

//============================================

void ImageRequestOperation::SetRequestOwner(IRestRequestOwner *owner) {
	__pRequestOwner = owner;
}

void ImageRequestOperation::AddImageRequestListener(IImageRequestListener *pImageRequestListener) {
	__pImageRequestListener = pImageRequestListener;
}

//============================================

void ImageRequestOperation::CheckCompletionAndCleanUp() {
	if (__isComplited) {
		__pRequestOwner->OnCompliteN(this);
	} else {
		__isComplited = true;
	}
}

//===========================================

void
ImageRequestOperation::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("ImageRequestOperation::OnTransactionReadyToRead");

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if (pHttpHeader != null)
		{
			String* tempHeaderString = pHttpHeader->GetRawHeaderN();
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			AppLog("ImageRequestOperation::OnTransactionReadyToRead");
			if (__pByteBuffer == null) {
				AppLog("ImageRequestOperation::INIT");
				__pByteBuffer = new ByteBuffer();
				__pByteBuffer->Construct(availableBodyLen);
			} else {
				int newCapacity = __pByteBuffer->GetCapacity() + availableBodyLen;
				__pByteBuffer->ExpandCapacity(newCapacity);
			}

			__pByteBuffer->CopyFrom(*pBuffer);

			delete pBuffer;
			delete tempHeaderString;
		}
	}
}

void
ImageRequestOperation::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("ImageRequestOperation::OnTransactionAborted(%s)", GetErrorMessage(r));
	__pRequestOwner->OnCompliteN(this);
}

void
ImageRequestOperation::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("ImageRequestOperation::OnTransactionReadyToWrite");
}

void
ImageRequestOperation::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool authRequired)
{
	AppLog("ImageRequestOperation::OnTransactionHeaderCompleted");
}

void
ImageRequestOperation::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("ImageRequestOperation::OnTransactionCompleted");
	dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		Execute();
		__pRequestOwner->OnCompliteN(this);
	});
}

void
ImageRequestOperation::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{
	AppLog("ImageRequestOperation::OnTransactionCertVerificationRequiredN");

	httpTransaction.Resume();
	delete pCert;
}

bool
ImageRequestOperation::GetIsComplited() {
	return __isComplited;
}

void
ImageRequestOperation::Execute() {
	Image *pImage = new Image();
	pImage->Construct();

	BitmapPixelFormat pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
	ImageFormat format = IMG_FORMAT_JPG;

	if(__pUrl->EndsWith(L"jpg") or __pUrl->EndsWith(L"bmp") or __pUrl->EndsWith(L"gif"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
		format = IMG_FORMAT_JPG;
	}
	else if(__pUrl->EndsWith(L"png"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_ARGB8888;
		format = IMG_FORMAT_PNG;
	}

	AppLog("%d :: %d :: %d", format, pixelFormat, __pByteBuffer->GetCapacity());

	Bitmap *pBitmap = pImage->DecodeN(*__pByteBuffer, format, pixelFormat);

	result r = GetLastResult();

	if (r == E_INVALID_ARG) {
		AppLog("The specified pixel format is not supported.");
	} else if (r == E_INVALID_DATA) {
		AppLog("The specified input instance has invalid data.");
	}  else if (r ==  E_OVERFLOW) {
		AppLog("The specified input instance has overflowed.");
	}  else if (r ==  E_UNSUPPORTED_FORMAT) {
		AppLog("The specified format is not supported.");
	}  else if (r ==  E_OUT_OF_MEMORY	) {
		AppLog("The memory is insufficient.");
	}  else if (r ==  E_OBJ_NOT_FOUND) {
		AppLog("The specified image buffer cannot be found.");
	}  else if (r ==  E_OUT_OF_RANGE	) {
		AppLog("E_OUT_OF_RANGE");
	}

	if (__pImageRequestListener && r == E_SUCCESS) {
		__pImageRequestListener->OnImageLoadedN(pBitmap);
		if (pBitmap) {
			ImageCache::StoreImageForKey(pBitmap, __pUrl);
		}
	}

	delete pImage;
}

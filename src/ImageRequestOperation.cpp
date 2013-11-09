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

	__isComplited = false;
}

ImageRequestOperation::~ImageRequestOperation() {
	AppLogDebug("ImageRequestOperation::~ImageRequestOperatio");
	delete __pHttpTransaction;
	__pHttpTransaction = null;
	delete __pUrl;
	__pUrl = null;
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


			Bitmap *pBitmap = pImage->DecodeN(*pBuffer, format, pixelFormat);

			if (__pImageRequestListener) {
				__pImageRequestListener->OnImageLoadedN(pBitmap);
				if (pBitmap) {
					ImageCache::StoreImageForKey(pBitmap, __pUrl);
				}

				CheckCompletionAndCleanUp();
			} else {
				CheckCompletionAndCleanUp();
			}

			delete tempHeaderString;
			delete pImage;
		}
	}
}

void
ImageRequestOperation::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("ImageRequestOperation::OnTransactionAborted(%s)", GetErrorMessage(r));
	CheckCompletionAndCleanUp();
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
	CheckCompletionAndCleanUp();
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

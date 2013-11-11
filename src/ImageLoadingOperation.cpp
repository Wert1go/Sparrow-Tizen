/*
 * ImageLoadingOperation.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include "ImageLoadingOperation.h"

#include <FGraphics.h>
#include "Error.h"
#include "ImageRequestOperation.h"
#include "RestClient.h"
#include "ImageCache.h"

#include "dispatch/dispatch.h"

using namespace Tizen::Base;

ImageLoadingOperation::ImageLoadingOperation(String *url) {
	__pUrl = url;
	__pImageRequestOperation = null;
	__pImageOperationListener = null;
	__pImageRequestOperation = new ImageRequestOperation(__pUrl);
	__lolprotector = 0;
}

ImageLoadingOperation::~ImageLoadingOperation() {
	if (__pImageRequestOperation) {
		delete __pImageRequestOperation;
	}

	__pUrl = null;
	__pImageRequestOperation = null;
	__pImageOperationListener = null;
}

void ImageLoadingOperation::Perform() {
	if (__lolprotector != 0) {
		AppLogDebug("Operation already running!");
		return;
	}
	__lolprotector++;

	//ОС требует, чтобы транзакция открывалась в главном потоке приложения

//	dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		Bitmap *pBitmap = ImageCache::LoadFromCacheForKeyN(__pUrl);
		if (pBitmap) {

			AppLogDebug("ImageLoadingOperation::OnImageLoadedN!!!");
			OnImageLoadedN(pBitmap);
			AppLogDebug("ImageLoadingOperation::OnImageLoadedN333");

		} else {
			AppLogDebug("ImageLoadingOperation::Begin loading");
			__pImageRequestOperation->AddImageRequestListener(this);
			RestClient::getInstance().PerformOperation(__pImageRequestOperation);
		}
//	});
}

void ImageLoadingOperation::Execute() {


}

void ImageLoadingOperation::OnImageLoadedN(Bitmap *pBitmap) {
	__pImageRequestOperation->AddImageRequestListener(null);
	__pImageRequestOperation = null;

	AppLogDebug("ImageLoadingOperation::OnImageLoadedN");

	if (pBitmap) {
		__pImageOperationListener->OnImageLoadedN(__pUrl, pBitmap);
	} else {
		AppLogDebug("Failed to load image: %S", __pUrl->GetPointer());
		__pImageOperationListener->OnErrorN(__pUrl, null);
	}
}

void ImageLoadingOperation::OnErrorN(Error *error) {
	__pImageOperationListener->OnErrorN(__pUrl, error);
	delete error;
}

void ImageLoadingOperation::AddImageOperationListener(IImageOperationListener *pImageOperationListener) {
	__pImageOperationListener = pImageOperationListener;
}

/*
 * ImageCache.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#include "ImageCache.h"
#include "Util.h"

#include <FApp.h>
#include <FMedia.h>
#include "ImageLoadingOperation.h"

using namespace Tizen::Media;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;


ImageCache::ImageCache() {
	__mutex.Create();

	__pUrlAndTargetMap = new HashMapT<String*, IImageLoadingListener*>();
	__pTargetAndUrlMap = new HashMapT<IImageLoadingListener*, String*>();
	__pUrlAndOperationMap = new HashMapT<String*, ImageLoadingOperation*>();
	__pPendingOperation = new HashMapT<String*, ImageLoadingOperation*>();

	__pUrlAndTargetMap->Construct(100, 0.75);
	__pTargetAndUrlMap->Construct(100, 0.75);
	__pUrlAndOperationMap->Construct(100, 0.75);
	__pPendingOperation->Construct(100, 0.75);

	__runningOperations = 0;

}

ImageCache::~ImageCache() {

}

void
ImageCache::LoadImageForTarget(String *url, IImageLoadingListener *target) {
	//AppLogDebug("ImageCache::LoadImageForTarget");
//	return;
	if (url == null || url->GetPointer() == 0 || url->GetLength() < 10) {
		return;
	}

	AppLogDebug("%S :: %d", url->GetPointer(), url->GetLength());

	if (CheckExistingOperationForUrl(url)) {
		//надо придумать, что делать при поптыке загрузить изображение до окончания 1-й попытки
	} else {
		ImageLoadingOperation *operation = new ImageLoadingOperation(url);
		operation->AddImageOperationListener(this);

		__mutex.Acquire();
		__pUrlAndTargetMap->Add(url, target);
		__pTargetAndUrlMap->Add(target, url);
		__pUrlAndOperationMap->Add(url, operation);

		__mutex.Release();
//		AppLogDebug("LoadImageForTarget::LoadImageForTarget");
		if (__runningOperations < 3) {

			__mutex.Acquire();
			__runningOperations++;
			__mutex.Release();
			operation->Perform();
		} else {
			//AppLogDebug("__pPendingOperation->Add <<<<<<<<<<<<<<<<<<");
			__pPendingOperation->Add(url, operation);
		}
	}
}

void
ImageCache::CancelLoadingForTarget(IImageLoadingListener *target) {
	__mutex.Acquire();
	bool result = false;

	__pTargetAndUrlMap->ContainsKey(target, result);
	if (result) {
		String *url;
		__pTargetAndUrlMap->GetValue(target, url);

		if (url) {
			__pUrlAndTargetMap->Remove(url);
		}
		__pTargetAndUrlMap->Remove(target);

	} else {
		//ничего не делаем, раз ключа нет
	}

	__mutex.Release();
}

bool
ImageCache::CheckExistingOperationForUrl(String *url) {

	bool result = false;

	__mutex.Acquire();
	__pUrlAndTargetMap->ContainsKey(url, result);
	__mutex.Release();

	return result;
}

void
ImageCache::CheckPendingOperationsAndRun() {
	//return;
	if (__pPendingOperation->GetCount() > 0) {
		IListT<String *> *keysList = __pPendingOperation->GetKeysN();

		String *key;
		keysList->GetAt(0, key);

		ImageLoadingOperation *operation;
		__pPendingOperation->GetValue(key, operation);

		if (operation) {
			operation->Perform();
		}

		__pPendingOperation->Remove(key);

		delete keysList;
	}
}

void
ImageCache::FinishOperationForUrl(String *url) {
	__mutex.Acquire();
	bool result = false;
	__pUrlAndTargetMap->ContainsKey(url, result);

	if (result) {
		IImageLoadingListener *target;
		__pUrlAndTargetMap->GetValue(url, target);

		if (target) {
			__pTargetAndUrlMap->Remove(target);
		}

		__pUrlAndTargetMap->Remove(url);
	}

	result = false;
	__pUrlAndOperationMap->ContainsKey(url, result);
	if (result) {
		ImageLoadingOperation *operation;
		__pUrlAndOperationMap->GetValue(url, operation);

		__pUrlAndOperationMap->Remove(url);

		if (operation) {
			delete operation;
		}

		//delete url;
	}

	//AppLog("FinishOperationForUrl");
	__runningOperations--;
	CheckPendingOperationsAndRun();
	__mutex.Release();
}

void ImageCache::DispatchImage(String *url, Bitmap *bitmap) {
	__mutex.Acquire();
	bool result = false;
	__pUrlAndTargetMap->ContainsKey(url, result);

	if (result) {
		IImageLoadingListener *target;
		__pUrlAndTargetMap->GetValue(url, target);
		if (target) {
			target->OnImageLoadedN(bitmap);
		}
	}
	__mutex.Release();
}

// ******************** CALLBACKS *********************** //

void
ImageCache::OnImageLoadedN(String *url, Bitmap *result) {
	DispatchImage(url, result);
	FinishOperationForUrl(url);
}

void
ImageCache::OnErrorN(String *url, Error *error) {
	FinishOperationForUrl(url);
}

// ******************** UTILS | STATIC *********************** //

bool
ImageCache::StoreImageForKey(Bitmap *pBitmap, String *url) {
	BitmapPixelFormat pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
	ImageFormat format = IMG_FORMAT_JPG;

	//AppLogDebug("ImageCache::StoreImageForKey Begin");

	if(url->EndsWith(L"jpg") or url->EndsWith(L"bmp") or url->EndsWith(L"gif"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
		format = IMG_FORMAT_JPG;
	}
	else if(url->EndsWith(L"png"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_ARGB8888;
		format = IMG_FORMAT_PNG;
	}

	Image *image = new Image();
	image->Construct();
	String *key = ImageCache::CacheKeyForUrlN(url);
	String path = Tizen::App::App::GetInstance()->GetAppDataPath() + key->GetPointer();

	AppLogDebug("Store image to path: %S", path.GetPointer());

	image->EncodeToFile(*pBitmap, format, path, true);

	//AppLogDebug("ImageCache::StoreImageForKey End");
	delete key;
	delete image;

	return true;
}

Bitmap *
ImageCache::LoadFromCacheForKeyN(String *url) {
	BitmapPixelFormat pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
	ImageFormat format = IMG_FORMAT_JPG;

	if(url->EndsWith(L"jpg") or url->EndsWith(L"bmp") or url->EndsWith(L"gif"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_RGB565;
		format = IMG_FORMAT_JPG;
	}
	else if(url->EndsWith(L"png"))
	{
		pixelFormat = BITMAP_PIXEL_FORMAT_ARGB8888;
		format = IMG_FORMAT_PNG;
	}

	Image *image = new Image();
	image->Construct();

	String *key = ImageCache::CacheKeyForUrlN(url);
	String path = Tizen::App::App::GetInstance()->GetAppDataPath() + key->GetPointer();

	AppLogDebug("Load from path:: %S", path.GetPointer());

	Bitmap *pBitmap = image->DecodeN(path.GetPointer(), pixelFormat);

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

	if (r != E_SUCCESS) {
		AppLogDebug("Loading image from path:: %S FAILED", path.GetPointer());
		pBitmap = null;
	}

	delete key;
	delete image;

	return pBitmap;
}


String *
ImageCache::CacheKeyForUrlN(String *url) {
	return Util::MD5N(url);
}

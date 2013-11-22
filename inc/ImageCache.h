/*
 * ImageCache.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef IMAGECACHE_H_
#define IMAGECACHE_H_

#include <FBase.h>
#include <FGraphics.h>
#include "IImageLoadingListener.h"
#include "ImageLoadingOperation.h"
#include "IImageOperationListener.h"
#include "Error.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Base::Runtime;

class ImageCache
 : IImageOperationListener
{
public:

	static ImageCache& getInstance()
    {
    	static ImageCache	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

private:
	ImageCache();
	virtual ~ImageCache();
	ImageCache(ImageCache const&);              // Don't Implement
	void operator = (ImageCache const&);

public:
	//sync
	static bool StoreImageForKey(Bitmap *image, String *url);
	//sync
	static Bitmap *LoadFromCacheForKeyN(String *url);
	//util
	static String *CacheKeyForUrlN(String *url);

	void LoadImageForTarget(String *url, IImageLoadingListener *target, Integer *code = new Integer(0));
	void CancelLoadingForTarget(IImageLoadingListener *target);

private:
	virtual void OnImageLoadedN(String *url, Bitmap *result);
	virtual void OnErrorN(String *url, Error *error);

	bool CheckExistingOperationForUrl(String *url);
	void CheckPendingOperationsAndRun();
	void FinishOperationForUrl(String *url);
	void DispatchImage(String *url, Bitmap *bitmap);

private:
	HashMapT<String*, IImageLoadingListener*> *__pUrlAndTargetMap;
	MultiHashMapT<IImageLoadingListener*, String*> *__pTargetAndUrlMap;
	HashMapT<String*, ImageLoadingOperation*> *__pUrlAndOperationMap;
	HashMapT<String*, Integer*> *__pUrlAndCodeMap;
	HashMapT<String*, ImageLoadingOperation*> *__pPendingOperation;
	Mutex __mutex;
	int __runningOperations;

};

#endif /* IMAGECACHE_H_ */

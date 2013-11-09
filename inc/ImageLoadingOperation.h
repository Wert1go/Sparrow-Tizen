/*
 * ImageLoadingOperation.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef IMAGELOADINGOPERATION_H_
#define IMAGELOADINGOPERATION_H_

#include <FGraphics.h>
#include "Error.h"
#include "IImageOperationListener.h"
#include "ImageRequestOperation.h"
#include "IImageRequestListener.h"

using namespace Tizen::Base;

class ImageLoadingOperation
 : IImageRequestListener
{
public:
	ImageLoadingOperation(String *url);
	virtual ~ImageLoadingOperation();

private:
	virtual void OnImageLoadedN(Bitmap *result);
	virtual void OnErrorN(Error *error);

public:
	void AddImageOperationListener(IImageOperationListener *pImageOperationListener);
	void Perform();

private:
	void Execute();

private:
	String *__pUrl;
	IImageOperationListener *__pImageOperationListener;
	ImageRequestOperation *__pImageRequestOperation;

};

#endif /* IMAGELOADINGOPERATION_H_ */

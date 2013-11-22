/*
 * IImageLoadingListener.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef IIMAGELOADINGLISTENER_H_
#define IIMAGELOADINGLISTENER_H_

#include <FGraphics.h>
#include "Error.h"

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class IImageLoadingListener {
public:
	virtual ~IImageLoadingListener(void){};

	virtual void OnImageLoadedN(Bitmap *result, Integer *code){};
	virtual void OnErrorN(Error *error){};
};


#endif /* IIMAGELOADINGLISTENER_H_ */

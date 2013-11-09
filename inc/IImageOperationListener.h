/*
 * IImageOperationListener.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef IIMAGEOPERATIONLISTENER_H_
#define IIMAGEOPERATIONLISTENER_H_

#include <FGraphics.h>
#include "Error.h"

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class IImageOperationListener {
public:
	virtual ~IImageOperationListener(void){};

	virtual void OnImageLoadedN(String *url, Bitmap *result){};
	virtual void OnErrorN(String *url, Error *error){};
};



#endif /* IIMAGEOPERATIONLISTENER_H_ */

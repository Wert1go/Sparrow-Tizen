/*
 * IImageRequestListener.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef IIMAGEREQUESTLISTENER_H_
#define IIMAGEREQUESTLISTENER_H_

#include <FGraphics.h>
#include "Error.h"

using namespace Tizen::Graphics;

class IImageRequestListener {
public:
	virtual ~IImageRequestListener(void){};

	virtual void OnImageLoadedN(Bitmap *result){};
	virtual void OnErrorN(Error *error){};
};

#endif /* IIMAGEREQUESTLISTENER_H_ */

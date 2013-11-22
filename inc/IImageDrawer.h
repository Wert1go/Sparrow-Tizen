/*
 * IImageDrawer.h
 *
 *  Created on: Nov 22, 2013
 *      Author: developer
 */

#ifndef IIMAGEDRAWER_H_
#define IIMAGEDRAWER_H_

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class IImageDrawer {
public:
	virtual ~IImageDrawer(void){};

	virtual void DrawImageFromUrlInRect(String *imageUrl, Rectangle rect){};
};


#endif /* IIMAGEDRAWER_H_ */

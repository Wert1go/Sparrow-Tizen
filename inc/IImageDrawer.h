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

class MAttachment;

class IImageDrawer {
public:
	virtual ~IImageDrawer(void){};

	virtual void DrawImageFromUrlInRect(String *imageUrl, Rectangle rect){};
	virtual void DrawAttachmentFromUrlInRect(String *imageUrl, Rectangle rect, MAttachment *attachment){};
};


#endif /* IIMAGEDRAWER_H_ */

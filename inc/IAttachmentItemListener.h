/*
 * IAttachmentItemListener.h
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#ifndef IATTACHMENTITEMLISTENER_H_
#define IATTACHMENTITEMLISTENER_H_


class MAttachment;

class IAttachmentItemListener {
public:
	virtual ~IAttachmentItemListener(void){};

	virtual void DeleteButtonPressedAtIndex(int index) = 0;
};


#endif /* IATTACHMENTITEMLISTENER_H_ */

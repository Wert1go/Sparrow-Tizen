/*
 * IAttachmentListener.h
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#ifndef IATTACHMENTLISTENER_H_
#define IATTACHMENTLISTENER_H_

class MAttachment;

class IAttachmentListener {
public:
	virtual ~IAttachmentListener(void){};

	virtual void OnSuccessN(MAttachment *attachment, int uid){};
	virtual void OnErrorN(Error *error, MAttachment *attachment, int uid){};
	virtual void OnProgressChanged(MAttachment *attachment, int progress, int uid){};
};


#endif /* IATTACHMENTLISTENER_H_ */

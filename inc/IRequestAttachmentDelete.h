/*
 * IRequestAttachmentDelete.h
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#ifndef IREQUESTATTACHMENTDELETE_H_
#define IREQUESTATTACHMENTDELETE_H_

class IRequestAttachmentDelete {
public:
	virtual ~IRequestAttachmentDelete(void){};

	virtual void RequestAttachmentDelete(MAttachment *attachment, int index) = 0;
};


#endif /* IREQUESTATTACHMENTDELETE_H_ */

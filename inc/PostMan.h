/*
 * PostMan.h
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#ifndef POSTMAN_H_
#define POSTMAN_H_

#include "IMessageOwner.h"
#include "IMessageDeliveryListener.h"
#include "PostMessageOperation.h"
#include "MMessage.h"
#include "IAttachmentListener.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

class MAttachment;
class ImageAttachmentOperation;

class PostMan
 : public IMessageOwner
 , public IAttachmentListener
{
public:
public:
	static PostMan& getInstance()
    {
    	static PostMan	instance;
    	return instance;
	}
private:
	PostMan();
	virtual ~PostMan();
	PostMan(PostMan const&);
	void operator = (PostMan const&);

private:
	virtual void OnCompliteN(PostMessageOperation *operation);

public:
	bool ValidateIncomingMessage(MMessage *pMessage);
	bool CheckPermissionToSendFromUser(int userId);
	void SendMessageFromUserWithListener(MMessage *pMessage, int userId, IMessageDeliveryListener *pListener);
	void RemoveListenerForUser(int userId);

	void AddAttachmentWithUid(MAttachment *attachment, int uid);
	void RemoveAttachmentAtIndexWithUid(int index, int uid);

	virtual void OnSuccessN(MAttachment *attachment, int uid);
	virtual void OnErrorN(Error *error, MAttachment *attachment, int uid);
	virtual void OnProgressChanged(MAttachment *attachment, int progress, int uid);

	IAttachmentListener *__pAttachmentListener;

	LinkedList *GetAttachmentsForUid(int uid);

private:
	Mutex __mutex;
	HashMapT<Integer*, IMessageDeliveryListener*> *__pUidToListenerMap;
	HashMapT<Integer*, PostMessageOperation*> *__pUidToOperationMap;

	HashMapT<Integer*, LinkedList*> *__pUidToAttachmentsMap;
	HashMapT<Integer*, LinkedList*> *__pUidToAttachmentOperationsMap;

private:

	Integer *KeyForUid(int userId);
	Integer *KeyForAttachmentUid(int userId);

	ImageAttachmentOperation *GetOperationForAttachment(Integer *key, MAttachment *attachment);


	//руководит отправкой сообщений на сервер
	//временные сообщения?
};

#endif /* POSTMAN_H_ */

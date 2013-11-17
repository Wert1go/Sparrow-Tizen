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

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

class PostMan
 : public IMessageOwner
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

private:
	Mutex __mutex;
	HashMapT<Integer*, IMessageDeliveryListener*> *__pUidToListenerMap;
	HashMapT<Integer*, PostMessageOperation*> *__pUidToOperationMap;

private:

	Integer *KeyForUid(int userId);


	//руководит отправкой сообщений на сервер
	//временные сообщения?
};

#endif /* POSTMAN_H_ */

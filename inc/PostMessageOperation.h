/*
 * PostMessageOperation.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef POSTMESSAGEOPERATION_H_
#define POSTMESSAGEOPERATION_H_

#include "IRestRequestListener.h"

class RestResponse;
class Error;
class IMessageOwner;
class IMessageDeliveryListener;
class MMessage;
class RestRequestOperation;

class PostMessageOperation
 : IRestRequestListener
{
public:
	PostMessageOperation();
	virtual ~PostMessageOperation();

public:
	void SetMessageOwner(IMessageOwner *pMessageOwner);
	void SetDeliveryListener(IMessageDeliveryListener *pDeliveryListener);

	void SetMessage(MMessage *message);
	void Perform();

	int __operationId;

private:
	void OnSuccessN(RestResponse *result);
	void OnErrorN(Error *error);

	void SendMessage();

private:
	MMessage *__pMessage;
	IMessageOwner *__pMessageOwner;
	IMessageDeliveryListener *__pDeliveryListener;

	RestRequestOperation *__pSendMessageOperation;
};

#endif /* POSTMESSAGEOPERATION_H_ */

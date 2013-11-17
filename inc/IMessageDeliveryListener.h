/*
 * IMessageDeliveryListener.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef IMESSAGEDELIVERYLISTENER_H_
#define IMESSAGEDELIVERYLISTENER_H_


#include "Error.h"
#include "MMessage.h"

class IMessageDeliveryListener {
public:
	virtual ~IMessageDeliveryListener(void){};

	virtual void OnMessageDelivered(int userId, MMessage *message){};
	virtual void OnErrorN(Error *error){};
};


#endif /* IMESSAGEDELIVERYLISTENER_H_ */

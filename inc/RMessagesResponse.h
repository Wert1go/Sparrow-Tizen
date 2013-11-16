/*
 * RMessagesResponse.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef RMESSAGESRESPONSE_H_
#define RMESSAGESRESPONSE_H_

#include "RestResponse.h"

using namespace Tizen::Base::Collection;

class RMessagesResponse
 : public RestResponse
{
public:
	RMessagesResponse();
	virtual ~RMessagesResponse();

	void SetMessages(LinkedList *messages);
	LinkedList *GetMessages();

private:
	LinkedList *__pMessages;
};

#endif /* RMESSAGESRESPONSE_H_ */

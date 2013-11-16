/*
 * LongPollResponse.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLRESPONSE_H_
#define LONGPOLLRESPONSE_H_

#include "RestResponse.h"

#include <FBase.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class LongPollResponse
 : public RestResponse
{
public:
	LongPollResponse();
	virtual ~LongPollResponse();

	void SetUpdates(LinkedList *updates);
	LinkedList *GetUpdates();

	void SetTS(String *ts);
	String *GetTS();

private:
	LinkedList *__pUpdates;
	String *__pTs;

};

#endif /* LONGPOLLRESPONSE_H_ */

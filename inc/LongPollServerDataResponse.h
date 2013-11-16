/*
 * LongPollServerDataResponse.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLSERVERDATARESPONSE_H_
#define LONGPOLLSERVERDATARESPONSE_H_

#include "RestResponse.h"

#include <FBase.h>

using namespace Tizen::Base;

class LongPollServerDataResponse
 : public RestResponse
{
public:
	LongPollServerDataResponse();
	virtual ~LongPollServerDataResponse();

public:
	String * GetKey();
	String * GetServer();
	String * GetTS();

	void SetKey(String *pKey);
	void SetServer(String *pServer);
	void SetTS(String *pTS);

private:
	String *__pKey;
	String *__pServer;
	String *__pTS;
};

#endif /* LONGPOLLSERVERDATARESPONSE_H_ */

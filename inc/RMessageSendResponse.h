/*
 * RMessageSendResponse.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef RMESSAGESENDRESPONSE_H_
#define RMESSAGESENDRESPONSE_H_

#include "RestResponse.h"

class RMessageSendResponse
 : public RestResponse
{
public:
	RMessageSendResponse();
	virtual ~RMessageSendResponse();

	int __mid;
};

#endif /* RMESSAGESENDRESPONSE_H_ */

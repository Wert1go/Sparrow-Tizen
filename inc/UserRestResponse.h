/*
 * UserRestResponse.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef USERRESTRESPONSE_H_
#define USERRESTRESPONSE_H_

#include "RestResponse.h"
#include "MUser.h"

class UserRestResponse
 : public RestResponse
{
public:
	UserRestResponse();
	virtual ~UserRestResponse();

	void SetUser(MUser *user);
	MUser *GetUser();

private:
	MUser *__user;
};

#endif /* USERRESTRESPONSE_H_ */

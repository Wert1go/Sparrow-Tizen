/*
 * UserRestResponse.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef USERRESTRESPONSE_H_
#define USERRESTRESPONSE_H_

#include <FBase.h>

using namespace Tizen::Base::Collection;

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


	void SetUsers(LinkedList *users);
	LinkedList *GetUsers();

private:
	MUser *__user;
	LinkedList *__pUsers;

};

#endif /* USERRESTRESPONSE_H_ */

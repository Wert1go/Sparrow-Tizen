/*
 * UserRestResponse.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include "UserRestResponse.h"

UserRestResponse::UserRestResponse() {
	// TODO Auto-generated constructor stub
	__user = null;
}

UserRestResponse::~UserRestResponse() {
	// TODO Auto-generated destructor stub
}

void UserRestResponse::SetUser(MUser *user) {
	__user = user;
}

MUser* UserRestResponse::GetUser() {
	return __user;
}

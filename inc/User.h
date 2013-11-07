/*
 * User.h
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#ifndef USER_H_
#define USER_H_

#include <FBase.h>

using namespace Tizen::Base;

#include "Bean.h"

class User
 : Bean
{
public:
	User();
	virtual ~User();

	static User* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);

private:
	String *__firstName;
	String *__lastName;
	String __phoneNumber;
	String __photo;
	Integer __uid;
	Integer __sex;
	Long __lastSeen;

	String __photoMediumRec;

public:
	String * GetFirstName();
	String * GetLastName();

	void SetFirstName(String *firstName);
	void SetLastName(String *lastName);
};

#endif /* USER_H_ */

/*
 * User.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#include "User.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

User::User() {
	// TODO Auto-generated constructor stub

}

User::~User() {
	// TODO Auto-generated destructor stub
}


String *
User::GetFirstName() {
	return __firstName;
}

String *
User::GetLastName() {
	return __lastName;
}

void User::SetFirstName(String *firstName) {
	__firstName = firstName;
}

void User::SetLastName(String *lastName) {
	__lastName = lastName;
}

User *
User::CreateFromJsonN(const Tizen::Web::Json::JsonObject &pUserObject) {

	User *user = new (std::nothrow) User();

	JsonString* pKeyFirstName = new JsonString(L"first_name");
	JsonString* pKeyLastName = new JsonString(L"last_name");

	IJsonValue* pValFirstName = null;
	IJsonValue* pValLastName = null;

	pUserObject.GetValue(pKeyFirstName, pValFirstName);
	pUserObject.GetValue(pKeyLastName, pValLastName);

	JsonString *firstName = static_cast< JsonString* >(pValFirstName);
	JsonString *lastName = static_cast< JsonString* >(pValLastName);

	String *pFirstName = new String(firstName->GetPointer());
	String *pLastName = new String(lastName->GetPointer());

	user->SetFirstName(pFirstName);
	user->SetLastName(pLastName);

	delete pKeyFirstName;
	delete pKeyLastName;

	return user;
}

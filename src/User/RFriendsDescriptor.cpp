/*
 * RFriendsDescriptor.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: developer
 */

#include "RFriendsDescriptor.h"

#include "UserRestResponse.h"
#include "MUserDao.h"

RFriendsDescriptor::RFriendsDescriptor() {
	__isPresisterActive = true;
}

RFriendsDescriptor::~RFriendsDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RFriendsDescriptor::performObjectMappingN(JsonObject* pObject) {

	UserRestResponse *response = new UserRestResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseObject = null;
	pObject->GetValue(pKeyResponse, pValResponseObject);

	JsonObject *pResponseObject = static_cast< JsonObject* >(pValResponseObject);

	JsonString* pKeyUsers = new JsonString(L"items");
	IJsonValue* pValUsersArray = null;

	pResponseObject->GetValue(pKeyUsers, pValUsersArray);

	JsonArray *pArray = static_cast< JsonArray* >(pValUsersArray);

	LinkedList *users = new LinkedList();
	AppLog("Begin mapping");

	for (int index = 0; index < pArray->GetCount(); index++) {
		IJsonValue* pUserObjectValue = null;
		pArray->GetAt(index, pUserObjectValue);

		JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

		MUser *user = MUser::CreateFromJsonN(*pUserObject);
		users->Add(user);
	}
	response->SetUsers(users);
	AppLog("End mapping");
	if (__isPresisterActive) {
		MUserDao::getInstance().Save(users, true);
	}

	delete pKeyResponse;
	delete pKeyUsers;

	return response;
}

void
RFriendsDescriptor::SetPersisterActive(bool state) {
	__isPresisterActive = state;
}

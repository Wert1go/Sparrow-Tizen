/*
 * UserDescriptor.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include "UserDescriptor.h"

#include "UserRestResponse.h"
#include "MUserDao.h"

UserDescriptor::UserDescriptor() {
	// TODO Auto-generated constructor stub

}

UserDescriptor::~UserDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *UserDescriptor::performObjectMappingN(JsonObject* pObject) {

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
//
//		if (index == 0) {
//			response->SetUser(user);
//		}

		users->Add(user);

		AppLogDebug("%S", user->GetMiniPhoto()->GetPointer());
	}
	response->SetUsers(users);

	AppLog("Complite mapping %d", users->GetCount());
//	MUserDao::getInstance().Save(users);
	AppLog("Complite saving");
	delete pKeyResponse;
	delete pKeyUsers;

	return response;
}

/*
 * RContactsDescriptor.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: developer
 */

#include "RContactsDescriptor.h"

#include "UserRestResponse.h"
#include "MUserDao.h"

RContactsDescriptor::RContactsDescriptor() {
	// TODO Auto-generated constructor stub

}

RContactsDescriptor::~RContactsDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RContactsDescriptor::performObjectMappingN(JsonObject* pObject) {

	UserRestResponse *response = new UserRestResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValUsersArray = null;
	pObject->GetValue(pKeyResponse, pValUsersArray);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}
	JsonArray *pArray = static_cast< JsonArray* >(pValUsersArray);

	LinkedList *users = new LinkedList();
	AppLog("Begin mapping");

	for (int index = 0; index < pArray->GetCount(); index++) {
		IJsonValue* pUserObjectValue = null;
		pArray->GetAt(index, pUserObjectValue);

		JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

		MUser *user = MUser::CreateFromJsonN(*pUserObject);

		if (index == 0) {
			response->SetUser(user);
		}

		users->Add(user);
	}
	response->SetUsers(users);

	MUserDao::getInstance().Save(users);

	delete pKeyResponse;

	return response;
}

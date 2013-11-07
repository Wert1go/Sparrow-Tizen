/*
 * UserDescriptor.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include "UserDescriptor.h"

#include "UserRestResponse.h"

UserDescriptor::UserDescriptor() {
	// TODO Auto-generated constructor stub

}

UserDescriptor::~UserDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *UserDescriptor::performObjectMappingN(JsonObject* pObject) {

	UserRestResponse *response = new UserRestResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseArray = null;
	pObject->GetValue(pKeyResponse, pValResponseArray);

	JsonArray *pArray = static_cast< JsonArray* >(pValResponseArray);

	if (pArray->GetCount() > 0) {
		IJsonValue* pUserObjectValue = null;
		pArray->GetAt(0, pUserObjectValue);

		JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

		User *user = User::CreateFromJsonN(*pUserObject);

		response->SetUser(user);

		AppLog("OnTransactionReadyToRead1");
	}

	return response;
}

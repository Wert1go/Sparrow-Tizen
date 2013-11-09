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
	IJsonValue* pValResponseArray = null;
	pObject->GetValue(pKeyResponse, pValResponseArray);

	JsonArray *pArray = static_cast< JsonArray* >(pValResponseArray);

	if (pArray->GetCount() > 0) {
		IJsonValue* pUserObjectValue = null;
		pArray->GetAt(0, pUserObjectValue);

		JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

		AppLog("Begin mapping");
		MUser *user = MUser::CreateFromJsonN(*pUserObject);

		AppLog("Begin Cache");
		MUserDao::getInstance().Save(user);
		AppLog("End Cache");
		response->SetUser(user);

		AppLog("OnTransactionReadyToRead1");
	}

	delete pKeyResponse;

	return response;
}

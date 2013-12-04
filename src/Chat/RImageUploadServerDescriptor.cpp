/*
 * RImageUploadServerDescriptor.cpp
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#include "RImageUploadServerDescriptor.h"
#include "RImageUploadServerResponse.h"

RImageUploadServerDescriptor::RImageUploadServerDescriptor() {
	// TODO Auto-generated constructor stub

}

RImageUploadServerDescriptor::~RImageUploadServerDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RImageUploadServerDescriptor::performObjectMappingN(JsonObject* pObject) {

	RImageUploadServerResponse *response = new RImageUploadServerResponse();

	JsonString* pKeyResponse = new JsonString(L"response");

	IJsonValue* pValResponseObject = null;
	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}

	JsonObject *pResponseObject = static_cast< JsonObject* >(pValResponseObject);

	JsonString* pKeyUploadUrl = new JsonString(L"upload_url");
	IJsonValue* pValUploadUrl = null;
	pResponseObject->GetValue(pKeyUploadUrl, pValUploadUrl);

	if (pValUploadUrl) {
		JsonString *pUploadUrl = static_cast<JsonString *>(pValUploadUrl);

		response->__pUploadServer = new String(pUploadUrl->GetPointer());
	}

	return response;

}

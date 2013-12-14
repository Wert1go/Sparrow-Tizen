/*
 * RUnreadCountDescriptor.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: developer
 */

#include "RUnreadCountDescriptor.h"
#include "RMessageSendResponse.h"

#include <FWeb.h>

using namespace Tizen::Web::Json;

RUnreadCountDescriptor::RUnreadCountDescriptor() {
	// TODO Auto-generated constructor stub

}

RUnreadCountDescriptor::~RUnreadCountDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RUnreadCountDescriptor::performObjectMappingN(JsonObject* pObject) {
	RMessageSendResponse *response = new RMessageSendResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseObject = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		delete pKeyResponse;
		response->SetError(new Error());
		return response;
	}

	if (!pValResponseObject) {
		response->SetError(new Error());
	} else {
		JsonObject *pResponseObject = static_cast<JsonObject *>(pValResponseObject);

		JsonString* pKeyCount = new JsonString(L"count");
		IJsonValue* pValCount = null;

		pResponseObject->GetValue(pKeyCount, pValCount);

		if (pValCount) {

			JsonNumber *pCountId = static_cast<JsonNumber *>(pValCount);
			response->__mid = pCountId->ToInt();
		} else {
			response->__mid = 0;
		}

		delete pKeyCount;
	}

	delete pKeyResponse;

	return response;
}

/*
 * RMessageSendDescriptor.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#include "RMessageSendDescriptor.h"
#include "RMessageSendResponse.h"

#include <FWeb.h>

using namespace Tizen::Web::Json;

RMessageSendDescriptor::RMessageSendDescriptor() {
	// TODO Auto-generated constructor stub

}

RMessageSendDescriptor::~RMessageSendDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RMessageSendDescriptor::performObjectMappingN(JsonObject* pObject) {
	RMessageSendResponse *response = new RMessageSendResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseObject = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);
	if (!pValResponseObject) {
		response->SetError(new Error());
	} else {
		JsonNumber *pResponseId = static_cast<JsonNumber *>(pValResponseObject);
		response->__mid = pResponseId->ToInt();
	}

	return response;
}

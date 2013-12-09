/*
 * RMessageSendExecuteDescriptor.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: developer
 */

#include "RMessageSendExecuteDescriptor.h"
#include "RMessageSendResponse.h"
#include "MMessage.h"
#include "MMessageDao.h"

RMessageSendExecuteDescriptor::RMessageSendExecuteDescriptor() {
	// TODO Auto-generated constructor stub

}

RMessageSendExecuteDescriptor::~RMessageSendExecuteDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RMessageSendExecuteDescriptor::performObjectMappingN(JsonObject* pObject) {
	RMessageSendResponse *response = new RMessageSendResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	JsonString* pKeyId = new JsonString(L"message_id");
	JsonString* pKeyMessage = new JsonString(L"message");

	IJsonValue* pValResponseObject = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}

	if (!pValResponseObject) {
		response->SetError(new Error());
	} else {
		JsonObject *pResponse = static_cast<JsonObject *>(pValResponseObject);

		IJsonValue* pValId = null;
		IJsonValue* pValMessage = null;

		pResponse->GetValue(pKeyId, pValId);
		pResponse->GetValue(pKeyMessage, pValMessage);

		JsonNumber *pMessageId = static_cast<JsonNumber *>(pValId);
		JsonObject *pMessageObject = static_cast<JsonObject *>(pValMessage);

		response->__mid = pMessageId->ToInt();

		MMessage *pMessage = null;
		if (pMessageObject) {
			pMessage = MMessage::CreateFromJsonN(*pMessageObject);
			if (pMessage) {
				MMessageDao::getInstance().Save(pMessage);
			}

			response->__pMessage = pMessage;
		}
	}

	delete pKeyResponse;
	delete pKeyId;
	delete pKeyMessage;

	return response;
}

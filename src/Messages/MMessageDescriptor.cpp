/*
 * MMessageDescriptor.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "MMessageDescriptor.h"
#include "RMessagesResponse.h"
#include "MMessage.h"
#include "MMessageDao.h"

MMessageDescriptor::MMessageDescriptor() {
	// TODO Auto-generated constructor stub

}

MMessageDescriptor::~MMessageDescriptor() {
	// TODO Auto-generated destructor stub
}


RestResponse *
MMessageDescriptor::performObjectMappingN(JsonObject* pObject) {
	RMessagesResponse *response = new RMessagesResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseObject = null;

	LinkedList *pMessages = new LinkedList();

	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!pValResponseObject) {
		response->SetError(new Error());
		return response;
	}

	JsonObject *pResponseObject = static_cast<JsonObject *>(pValResponseObject);

	JsonString* pKeyMessagesArray = new JsonString(L"items");
	IJsonValue* pValMessagesArray = null;

	pResponseObject->GetValue(pKeyMessagesArray, pValMessagesArray);

	JsonArray *pMessagesArray = static_cast<JsonArray*>(pValMessagesArray);

	for (int index = 0; index < pMessagesArray->GetCount(); index++) {
		IJsonValue* pValMessage = null;
		pMessagesArray->GetAt(index, pValMessage);
		JsonObject *pMessageObject = static_cast<JsonObject *>(pValMessage);

		MMessage *message = MMessage::CreateFromJsonN(*pMessageObject);
		AppAssert(message);

		pMessages->Add(message);
	}

	response->SetMessages(pMessages);

	MMessageDao::getInstance().Save(pMessages);

	delete pKeyResponse;
	delete pKeyMessagesArray;

	return response;
}

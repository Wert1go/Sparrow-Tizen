/*
 * LongPollDescriptor.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollDescriptor.h"
#include "RestResponse.h"
#include "LongPollResponse.h"
#include "LongPollObject.h"
#include <FWeb.h>

using namespace Tizen::Web::Json;

LongPollDescriptor::LongPollDescriptor() {
	// TODO Auto-generated constructor stub

}

LongPollDescriptor::~LongPollDescriptor() {

}

RestResponse *
LongPollDescriptor::performObjectMappingN(JsonObject* pObject) {
	LongPollResponse *response = new LongPollResponse();

	JsonString* pKeyTS = new JsonString(L"ts");
	JsonString* pKeyUpdates = new JsonString(L"updates");

	IJsonValue* pValKey = null;
	IJsonValue* pValUpdates = null;

	pObject->GetValue(pKeyTS, pValKey);
	pObject->GetValue(pKeyUpdates, pValUpdates);

	JsonNumber *pKey = static_cast<JsonNumber *>(pValKey);
	JsonArray *pUpdates = static_cast<JsonArray *>(pValUpdates);

	LinkedList *updatesList = new LinkedList();

	for (int index = 0; index < pUpdates->GetCount(); index++) {

		IJsonValue* pValData;
		pUpdates->GetAt(index, pValData);

		JsonArray *pData = static_cast<JsonArray *>(pValData);

		LongPollObject *pDataObject = LongPollObject::CreateFromJsonN(*pData);
		AppAssert(pDataObject);
		updatesList->Add(pDataObject);
	}

	String tsString;
	tsString.Format(25, L"%d", pKey->ToInt());

	response->SetTS(new String(tsString));
	response->SetUpdates(updatesList);

	delete pKeyTS;
	delete pKeyUpdates;

	return response;
}


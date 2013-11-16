/*
 * LongPollServerDataDescriptor.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollServerDataDescriptor.h"
#include "LongPollServerDataResponse.h"

using namespace Tizen::Web::Json;

LongPollServerDataDescriptor::LongPollServerDataDescriptor() {

}

LongPollServerDataDescriptor::~LongPollServerDataDescriptor() {

}

RestResponse *
LongPollServerDataDescriptor::performObjectMappingN(JsonObject* pObject) {
	LongPollServerDataResponse *response = new LongPollServerDataResponse();

	JsonString* pKeyResponse = new JsonString(L"response");

	IJsonValue* pValResponseObject = null;
	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!pValResponseObject) {
		response->SetError(new Error(UNKNOW));
		return response;
	}

	JsonObject *pResponseObject = static_cast< JsonObject* >(pValResponseObject);

	JsonString* pKeyKey = new JsonString(L"key");
	JsonString* pKeyServer = new JsonString(L"server");
	JsonString* pKeyTS = new JsonString(L"ts");

	IJsonValue* pValKey = null;
	IJsonValue* pValServer = null;
	IJsonValue* pValTS = null;

	pResponseObject->GetValue(pKeyKey, pValKey);
	pResponseObject->GetValue(pKeyServer, pValServer);
	pResponseObject->GetValue(pKeyTS, pValTS);

	JsonString *key = static_cast<JsonString*>(pValKey);
	JsonString *server = static_cast<JsonString*>(pValServer);
	JsonNumber *ts = static_cast<JsonNumber*>(pValTS);

	String *pKey = new String(key->GetPointer());
	String *pServer = new String(server->GetPointer());

	String tsString;
	tsString.Format(25, L"%d", ts->ToInt());

	String *pTS = new String(tsString);

	response->SetKey(pKey);
	response->SetServer(pServer);
	response->SetTS(pTS);

	delete pKeyResponse;
	delete pKeyServer;
	delete pKeyTS;

	return response;
}

/*
 * RImageUploadDataDescriptor.cpp
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#include "RImageUploadDataDescriptor.h"
#include "RImageUploadServerResponse.h"

RImageUploadDataDescriptor::RImageUploadDataDescriptor() {
	// TODO Auto-generated constructor stub

}

RImageUploadDataDescriptor::~RImageUploadDataDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RImageUploadDataDescriptor::performObjectMappingN(JsonObject* pObject) {

	RImageUploadServerResponse *response = new RImageUploadServerResponse();

	JsonString* pKeyResponse = new JsonString(L"photo");
	JsonString* pKeyServer = new JsonString(L"server");
	JsonString* pKeyHash = new JsonString(L"hash");

	IJsonValue* pValResponseObject = null;
	IJsonValue* pValServer = null;
	IJsonValue* pValHash = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);
	pObject->GetValue(pKeyServer, pValServer);
	pObject->GetValue(pKeyHash, pValHash);

	JsonString *pResponseObject = static_cast< JsonString* >(pValResponseObject);
	JsonNumber *pServer = static_cast< JsonNumber* >(pValServer);
	JsonString *pHash = static_cast< JsonString* >(pValHash);

	if (pResponseObject) {
		response->__pPhotoData = new String(pResponseObject->GetPointer());

		String serverString;
		serverString.Format(20, L"%d", pServer->ToInt());

		response->__pServerData = new String(serverString);
		response->__pHashData = new String(pHash->GetPointer());
	}

	delete pKeyResponse;
	delete pKeyServer;
	delete pKeyHash;

	return response;

}

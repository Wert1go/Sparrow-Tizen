/*
 * RImageSaveDescriptor.cpp
 *
 *  Created on: Dec 6, 2013
 *      Author: developer
 */

#include "RImageSaveDescriptor.h"
#include "RImageUploadServerResponse.h"
#include "MAttachment.h"
#include "Error.h"

RImageSaveDescriptor::RImageSaveDescriptor() {
	// TODO Auto-generated constructor stub

}

RImageSaveDescriptor::~RImageSaveDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RImageSaveDescriptor::performObjectMappingN(JsonObject* pObject) {
	RImageUploadServerResponse *response = new RImageUploadServerResponse();

	JsonString* pKeyResponse = new JsonString(L"response");

	IJsonValue* pValResponseObject = null;
	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}

	JsonArray *pResponseArray = static_cast< JsonArray* >(pValResponseObject);

	if (pResponseArray && pResponseArray->GetCount() > 0) {
		IJsonValue* pValAttach = null;
		pResponseArray->GetAt(0, pValAttach);

		if (pValAttach) {
			JsonObject *pAttach = static_cast<JsonObject *>(pValAttach);
			MAttachment *attachment = MAttachment::CreatePhotoFromJsonN(pAttach);
			if (attachment) {
				AppLogDebug("++++++++++++++++++++++++++++++++++++++++++++++");

				response->__pAttachment = attachment;
			}
		}
	}

	return response;
}

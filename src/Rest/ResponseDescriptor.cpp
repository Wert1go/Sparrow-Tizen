/*
 * ResponseDescriptor.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include "ResponseDescriptor.h"
#include "UiUpdateConstants.h"
#include <FWeb.h>
#include <FApp.h>

using namespace Tizen::Web::Json;

ResponseDescriptor::ResponseDescriptor() {
	// TODO Auto-generated constructor stub

}

ResponseDescriptor::~ResponseDescriptor() {
	// TODO Auto-generated destructor stub
}

bool
ResponseDescriptor::IsAuthorized(JsonObject* pObject) {

	JsonString* pKeyResponse = new JsonString(L"response");
	IJsonValue* pValResponseObject = null;
	pObject->GetValue(pKeyResponse, pValResponseObject);


	if (!pValResponseObject) {
		JsonString* pKeyError = new JsonString(L"error");
		JsonString* pKeyErrorCode = new JsonString(L"error_code");

		IJsonValue* pValErrorObject = null;
		IJsonValue* pValErrorCode = null;

		pObject->GetValue(pKeyError, pValErrorObject);

		JsonObject *pErrorObject = static_cast<JsonObject *>(pValErrorObject);

		if (pErrorObject) {
			pErrorObject->GetValue(pKeyErrorCode, pValErrorCode);
			JsonNumber *pErrorCode = static_cast<JsonNumber *>(pValErrorCode);

			if (pErrorCode && pErrorCode->ToInt() == 5) {
				Tizen::App::UiApp::GetInstance()->SendUserEvent(LOGOUT, 0);

				delete pKeyError;
				delete pKeyErrorCode;

				return false;
			}

		}

		delete pKeyError;
		delete pKeyErrorCode;

		return false;
	}

	return true;
}

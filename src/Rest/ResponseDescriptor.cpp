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

		Tizen::App::UiApp::GetInstance()->SendUserEvent(LOGOUT, 0);

		return false;
	}

	return true;
}

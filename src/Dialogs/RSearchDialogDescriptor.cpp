/*
 * RSearchDialogDescriptor.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: developer
 */

#include "RSearchDialogDescriptor.h"
#include "RDialogResponse.h"
#include "MDialog.h"
#include "MUser.h"
#include "MUserDao.h"

using namespace Tizen::Base::Utility;

RSearchDialogDescriptor::RSearchDialogDescriptor() {
	// TODO Auto-generated constructor stub

}

RSearchDialogDescriptor::~RSearchDialogDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RSearchDialogDescriptor::performObjectMappingN(JsonObject* pObject) {
	RDialogResponse *response = new RDialogResponse();

	LinkedList *pDialogs = new LinkedList();
	JsonString* pKeyResponse = new JsonString(L"response");

	IJsonValue* pValResponseObject = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}

	JsonObject *pResponseObject = static_cast< JsonObject* >(pValResponseObject);

	JsonString* pKeyUsers = new JsonString(L"users");
	JsonString* pKeyDialogs = new JsonString(L"dialogs");

	IJsonValue* pValUsersArray = null;
	IJsonValue* pValDialogsObject = null;

	pResponseObject->GetValue(pKeyUsers, pValUsersArray);
	pResponseObject->GetValue(pKeyDialogs, pValDialogsObject);

	if (!pValUsersArray || !pValDialogsObject) {
		response->SetError(new Error());
		return response;
	}

	JsonArray *pDialogsList = dynamic_cast<JsonArray*>(pValDialogsObject);
	if (dynamic_cast<JsonArray*>(pValDialogsObject) == 0 || !pDialogsList || pDialogsList->GetCount() == 0) {
		response->SetDialogs(pDialogs);
		return response;
	}

	LinkedList *pUsers = new LinkedList();
	JsonArray *pUsersList = static_cast<JsonArray *> (pValUsersArray);
	if (dynamic_cast<JsonArray*>(pValUsersArray) != 0 && pDialogsList && pDialogsList->GetCount() > 0) {
		for (int index = 0; index < pUsersList->GetCount(); index++) {
			IJsonValue* pUserObjectValue = null;
			pUsersList->GetAt(index, pUserObjectValue);

			JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

			MUser *user = MUser::CreateFromJsonN(*pUserObject);
			pUsers->Add(user);
		}

		MUserDao::getInstance().Save(pUsers);
	}

	for (int i = 0; i < pDialogsList->GetCount(); i++) {
		IJsonValue* pValDialogObject = null;
		pDialogsList->GetAt(i, pValDialogObject);
		JsonObject *pDialogObject = static_cast< JsonObject* >(pValDialogObject);

		MDialog *pDialog = MDialog::CreateSearchDialogFromJsonN(*pDialogObject);

		if (pDialog->__pType->Equals(L"chat", false) && pDialog->GetChatUids() && pUsers && pUsers->GetCount() > 0) {

			String delim(L",");

			StringTokenizer stringTokenizer(pDialog->GetChatUids()->GetPointer(), delim);

			String paramsString;
			while (stringTokenizer.HasMoreTokens())
			{
				stringTokenizer.GetNextToken(paramsString);
				int uid = -1;
				Integer::Parse(paramsString, uid);

				if (uid != -1) {
					for (int i = 0; i < pUsers->GetCount(); i++) {
						MUser *pUser = static_cast<MUser *>(pUsers->GetAt(i));
						if (pUser->GetUid() == uid) {
							if (!pDialog->GetUsers()) {
								pDialog->SetUsers(new LinkedList());
							}

							pDialog->GetUsers()->Add(pUser);
						}
					}
				}

			}

		}

		if (pDialog) {
			pDialogs->Add(pDialog);
		}
	}

	response->SetDialogs(pDialogs);

	delete pKeyUsers;
	delete pKeyDialogs;

	return response;
}

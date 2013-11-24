/*
 * MDialogsDescriptor.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "MDialogsDescriptor.h"
#include "RDialogResponse.h"
#include "MDialog.h"
#include "MDialogDao.h"
#include "MUser.h"
#include "MUserDao.h"
#include <typeinfo>

MDialogsDescriptor::MDialogsDescriptor() {
	// TODO Auto-generated constructor stub

}

MDialogsDescriptor::~MDialogsDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
MDialogsDescriptor::performObjectMappingN(JsonObject* pObject) {


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

//	JsonString* pKeyChatIds = new JsonString(L"chat_uids");
	JsonString* pKeyUsers = new JsonString(L"users");
	JsonString* pKeyMessages = new JsonString(L"messages");

//	IJsonValue* pValChatIdsArray = null;
	IJsonValue* pValUsersArray = null;
	IJsonValue* pValMessagesObject = null;
//	pResponseObject->GetValue(pKeyChatIds, pValChatIdsArray);
	pResponseObject->GetValue(pKeyUsers, pValUsersArray);
	pResponseObject->GetValue(pKeyMessages, pValMessagesObject);

	if (!pValUsersArray || !pValMessagesObject) {
		response->SetError(new Error());
		return response;
	}
//	JsonArray *pChatIds = static_cast<JsonArray *> (pValChatIdsArray);
	JsonString* pKeyChatId = new JsonString(L"chat_id");
	JsonString* pKeyUids = new JsonString(L"uids");

	JsonArray *pUsers = static_cast<JsonArray *> (pValUsersArray);
	JsonObject *pMessages = static_cast<JsonObject *> (pValMessagesObject);

	JsonString* pKeyMessagesArray = new JsonString(L"items");
	IJsonValue* pValMessagesArray = null;

	pMessages->GetValue(pKeyMessagesArray, pValMessagesArray);

	if (!pValMessagesArray) {
		response->SetError(new Error());
		return response;
	}
	JsonArray *pMessagesArray = dynamic_cast<JsonArray*>(pValMessagesArray);
	if (dynamic_cast<JsonArray*>(pValMessagesArray) == 0 || !pMessagesArray || pMessagesArray->GetCount() == 0) {
		response->SetDialogs(pDialogs);
		return response;
	}

	JsonString* pKeyMessageUserId = new JsonString(L"user_id");
	JsonString* pKeyUserId = new JsonString(L"id");
	for (int index = 0; index < pMessagesArray->GetCount(); index++) {
		IJsonValue* pMessageObjectValue = null;
		pMessagesArray->GetAt(index, pMessageObjectValue);
		JsonObject* pMessageObject = static_cast< JsonObject* >(pMessageObjectValue);

		IJsonValue* pValUserId = null;
		pMessageObject->GetValue(pKeyMessageUserId, pValUserId);
		JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);

		int messageUid = uid->ToInt();

		JsonObject *pUserObject = null;
		for (int nestedIndex = 0; nestedIndex < pUsers->GetCount(); nestedIndex++) {
			IJsonValue* pUserObjectValue = null;
			pUsers->GetAt(nestedIndex, pUserObjectValue);

			JsonObject* userObject = static_cast< JsonObject* >(pUserObjectValue);
			IJsonValue* pValUserId = null;
			userObject->GetValue(pKeyUserId, pValUserId);
			JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
			int userId = uid->ToInt();

			if (userId == messageUid) {
				pUserObject = userObject;
				break;
			}
		}

		if (pUserObject) {
			MDialog *dialog = MDialog::CreateFromJsonN(*pUserObject, *pMessageObject);
			pDialogs->Add(dialog);
		}
	}
	AppLog("test5");
	LinkedList *users = new LinkedList();
	for (int index = 0; index < pUsers->GetCount(); index++) {
		IJsonValue* pUserObjectValue = null;
		pUsers->GetAt(index, pUserObjectValue);

		JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);

		MUser *user = MUser::CreateFromJsonN(*pUserObject);
		users->Add(user);
	}

	AppLog("SAVE");
	MUserDao::getInstance().Save(users);
	AppLog("COMPLITE");
	MDialogDao::getInstance().Save(pDialogs);

	response->SetDialogs(pDialogs);

	delete pKeyResponse;
//	delete pKeyChatIds;
	delete pKeyUsers;
	delete pKeyMessages;
	delete pKeyMessagesArray;

	delete pKeyUserId;
	delete pKeyMessageUserId;

	delete pKeyChatId;
	delete pKeyUids;

	return response;
}

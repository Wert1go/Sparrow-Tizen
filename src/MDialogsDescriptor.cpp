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

	JsonObject *pResponseObject = static_cast< JsonObject* >(pValResponseObject);

	JsonString* pKeyChatIds = new JsonString(L"chat_ids");
	JsonString* pKeyUsers = new JsonString(L"users");
	JsonString* pKeyMessages = new JsonString(L"messages");

	IJsonValue* pValChatIdsArray = null;
	IJsonValue* pValUsersArray = null;
	IJsonValue* pValMessagesObject = null;

	pResponseObject->GetValue(pKeyChatIds, pValChatIdsArray);
	pResponseObject->GetValue(pKeyUsers, pValUsersArray);
	pResponseObject->GetValue(pKeyMessages, pValMessagesObject);

	JsonArray *pChatIds = static_cast<JsonArray *> (pValChatIdsArray);
	JsonArray *pUsers = static_cast<JsonArray *> (pValUsersArray);
	JsonObject *pMessages = static_cast<JsonObject *> (pValMessagesObject);

	JsonString* pKeyMessagesArray = new JsonString(L"items");
	IJsonValue* pValMessagesArray = null;

	pMessages->GetValue(pKeyMessagesArray, pValMessagesArray);

	JsonArray *pMessagesArray = static_cast<JsonArray*>(pValMessagesArray);

	if (pMessagesArray->GetCount() == 0) {
		response->SetDialogs(pDialogs);
		return response;
	}

	JsonString* pKeyMessageUserId = new JsonString(L"user_id");
	JsonString* pKeyUserId = new JsonString(L"id");
	AppLogDebug("1");
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

		MDialog *dialog = MDialog::CreateFromJsonN(*pUserObject, *pMessageObject);

		if (!dialog->GetText()) {
			AppLogDebug("ALLLAAAAAAARRMMMM");
		}

		AppLogDebug("text: %S", dialog->GetText()->GetPointer());
		pDialogs->Add(dialog);

	}

	response->SetDialogs(pDialogs);

	MDialogDao::getInstance().Save(pDialogs);

	delete pKeyResponse;
	delete pKeyChatIds;
	delete pKeyUsers;
	delete pKeyMessages;
	delete pKeyMessagesArray;

	delete pKeyUserId;
	delete pKeyMessageUserId;

	return response;
}

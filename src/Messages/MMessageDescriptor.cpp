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
#include "MUser.h"
#include "MUserDao.h"

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

	if (!this->IsAuthorized(pObject)) {
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

	JsonString* pKeyUsersArray = new JsonString(L"users");
	IJsonValue* pValUsersArray = null;

	pResponseObject->GetValue(pKeyUsersArray, pValUsersArray);

	JsonArray *pUsersArray = null;

	if (pValUsersArray) {
		pUsersArray = static_cast<JsonArray*>(pValUsersArray);
	}

	LinkedList *pUsers = new LinkedList();

	if (pUsersArray && pUsersArray->GetCount() > 0) {

		for (int i = 0; i < pUsersArray->GetCount(); i++) {
			IJsonValue* pValUser = null;
			pUsersArray->GetAt(i, pValUser);

			JsonObject *pUserObject = static_cast<JsonObject *>(pValUser);
			MUser *pUser = MUser::CreateFromJsonN(*pUserObject);

			pUsers->Add(pUser);
		}
		MUserDao::getInstance().Save(pUsers);

	}

	for(int i = 0; i < pMessages->GetCount(); i++) {
		MMessage *pMessage = static_cast<MMessage *>(pMessages->GetAt(i));
		this->LoadUsers(pMessage->__pFwd, pUsers);
	}

	delete pUsers;

	response->SetMessages(pMessages);

	MMessageDao::getInstance().Save(pMessages);

	delete pKeyResponse;
	delete pKeyMessagesArray;
	delete pKeyUsersArray;

	return response;
}


void
MMessageDescriptor::LoadUsers(IList * pFwdMessages, IList *pUsers) {
	if (pFwdMessages && pFwdMessages->GetCount() > 0) {
		for(int j = 0; j < pFwdMessages->GetCount(); j++) {
			MMessage *pFwd = static_cast<MMessage *>(pFwdMessages->GetAt(j));

			for(int k = 0; k < pUsers->GetCount(); k++) {
				MUser *pUser = static_cast<MUser *>(pUsers->GetAt(k));
				if (pFwd->GetUid() == pUser->GetUid()) {
					pFwd->__pUser = pUser;
					break;
				}
			}

			if (!pFwd->__pUser) {
				pFwd->__pUser = MUserDao::getInstance().GetUserN(pFwd->GetUid());
			}

			if (pFwd->__pFwd) {
				this->LoadUsers(pFwd->__pFwd, pUsers);
			}
		}
	}
}

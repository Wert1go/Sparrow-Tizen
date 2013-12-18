/*
 * RMessageSendExecuteDescriptor.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: developer
 */

#include "RMessageSendExecuteDescriptor.h"
#include "RMessageSendResponse.h"
#include "MMessage.h"
#include "MMessageDao.h"
#include "MUser.h"
#include "MUserDao.h"

RMessageSendExecuteDescriptor::RMessageSendExecuteDescriptor() {
	// TODO Auto-generated constructor stub

}

RMessageSendExecuteDescriptor::~RMessageSendExecuteDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RMessageSendExecuteDescriptor::performObjectMappingN(JsonObject* pObject) {
	RMessageSendResponse *response = new RMessageSendResponse();

	JsonString* pKeyResponse = new JsonString(L"response");
	JsonString* pKeyId = new JsonString(L"message_id");
	JsonString* pKeyMessage = new JsonString(L"message");

	IJsonValue* pValResponseObject = null;

	pObject->GetValue(pKeyResponse, pValResponseObject);

	if (!this->IsAuthorized(pObject)) {
		response->SetError(new Error());
		return response;
	}

	if (!pValResponseObject) {
		response->SetError(new Error());
	} else {
		JsonObject *pResponse = static_cast<JsonObject *>(pValResponseObject);


		JsonString* pKeyUsersArray = new JsonString(L"users");
		IJsonValue* pValUsersArray = null;

		pResponse->GetValue(pKeyUsersArray, pValUsersArray);

		JsonArray *pUsersArray = null;

		if (pValUsersArray) {
			pUsersArray = static_cast<JsonArray*>(pValUsersArray);
			if (dynamic_cast<JsonArray*>(pValUsersArray) == 0) {
				pUsersArray = null;
			}
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

		delete pKeyUsersArray;

		IJsonValue* pValId = null;
		IJsonValue* pValMessage = null;

		pResponse->GetValue(pKeyId, pValId);
		pResponse->GetValue(pKeyMessage, pValMessage);

		JsonNumber *pMessageId = static_cast<JsonNumber *>(pValId);
		JsonObject *pMessageObject = static_cast<JsonObject *>(pValMessage);

		response->__mid = pMessageId->ToInt();

		MMessage *pMessage = null;
		if (pMessageObject) {
			pMessage = MMessage::CreateFromJsonN(*pMessageObject);
			if (pMessage) {
				if (pMessage->__pFwd && pMessage->__pFwd->GetCount() > 0) {
					this->LoadUsers(pMessage->__pFwd, pUsers);
				}

				MMessageDao::getInstance().Save(pMessage);
			}

			response->__pMessage = pMessage;
		}
	}

	delete pKeyResponse;
	delete pKeyId;
	delete pKeyMessage;

	return response;
}

void
RMessageSendExecuteDescriptor::LoadUsers(IList * pFwdMessages, IList *pUsers) {
	if (pFwdMessages && pFwdMessages->GetCount() > 0) {
		for(int j = 0; j < pFwdMessages->GetCount(); j++) {
			MMessage *pFwd = static_cast<MMessage *>(pFwdMessages->GetAt(j));

			AppLog("LoadUserGet: %d", pFwd->GetMid());

			for(int k = 0; k < pUsers->GetCount(); k++) {
				MUser *pUser = static_cast<MUser *>(pUsers->GetAt(k));
				if (pFwd->GetUid() == pUser->GetUid()) {
					pFwd->__pUser = MUser::CreateFromUser(pUser);
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

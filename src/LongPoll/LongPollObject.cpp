/*
 * LongPollObject.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollObject.h"

#include "MMessage.h"
#include "MUser.h"

using namespace Tizen::Web::Json;

LongPollObject::LongPollObject() {
	__chatId = -1;
}

LongPollObject::~LongPollObject() {
	// TODO Auto-generated destructor stub
}

void
LongPollObject::SetType(int type) {
	__type = type;
}

int
LongPollObject::GetType() {
	return __type;
}

void
LongPollObject::SetUserId(int userId) {
	__userId= userId;
}

int
LongPollObject::GetUserId() {
	return __userId;
}

void
LongPollObject::SetChatId(int chatId) {
	__chatId = chatId;
}

int
LongPollObject::GetChatId() {
	return __chatId;
}

LongPollObject*
LongPollObject::CreateFromJsonN(const Tizen::Web::Json::JsonArray &jsonArray) {

	LongPollObject *resultObject = new LongPollObject();
	IJsonValue* pValType;
	IJsonValue* pValUserId;
	IJsonValue* pValChatId;

	JsonNumber *pUserId;
	JsonNumber *pChatId;

	jsonArray.GetAt(0, pValType);
	JsonNumber *pType = static_cast<JsonNumber *>(pValType);

	int type = pType->ToInt();

	resultObject->SetType(type);

	switch(type) {
	case LP_DELETE:

		break;
	case LP_FLAG_REPLACE:

			break;
	case LP_FLAG_ADD:

			break;
	case LP_FLAG_RESET: {
		IJsonValue *pValMsgId;
		jsonArray.GetAt(1, pValMsgId);

		JsonNumber *pMsgId = static_cast<JsonNumber *>(pValMsgId);

		resultObject->__messageId = pMsgId->ToInt();
	}
			break;
	case LP_MESSAGE_ADD: {
		IJsonValue *pValMsgId;
		jsonArray.GetAt(1, pValMsgId);

		JsonNumber *pMsgId = static_cast<JsonNumber *>(pValMsgId);

		resultObject->__messageId = pMsgId->ToInt();

		if (jsonArray.GetCount() >= 7) {
			IJsonValue *pValMsgText;
			jsonArray.GetAt(6, pValMsgText);

			if(pValMsgText) {
				JsonString *pMsgText = static_cast<JsonString *>(pValMsgText);
				String *pText = new String(pMsgText->GetPointer());
				if (pText && pText->GetLength() > 0) {
					pText->Replace(L"<br>", L"\n");

					pText->Replace(L"&quot;", L"\"");
					pText->Replace(L"&amp;", L"&");
					pText->Replace(L"&lt;", L"<");
					pText->Replace(L"&gt;", L">");
				}

				resultObject->__pText = pText;
			} else {
				resultObject->__pText = new String(L"");
			}
		}

	}
		break;
	case LP_MESSAGE_ADD_FULL: {
		resultObject->SetUsers(new LinkedList());

		IJsonValue *pValDataObject;
		jsonArray.GetAt(1, pValDataObject);

		JsonObject *pDataObject = static_cast<JsonObject *>(pValDataObject);

		JsonString *pKeyMessage = new JsonString(L"message");
		IJsonValue *pValMessage;
		pDataObject->GetValue(pKeyMessage, pValMessage);
		JsonObject *pMessageJson = static_cast<JsonObject *>(pValMessage);

		MMessage *pMessage = MMessage::CreateFromJsonLPN(*pMessageJson);
		AppAssert(pMessage);
		resultObject->SetMessage(pMessage);

		JsonString *pKeyUsers = new JsonString(L"profiles");
		IJsonValue *pValUsers;

		pDataObject->GetValue(pKeyUsers, pValUsers);
		JsonArray *pUsersArray = static_cast<JsonArray *>(pValUsers);

		for (int index = 0; index < pUsersArray->GetCount(); index++) {
			IJsonValue *pValUser;
			pUsersArray->GetAt(index, pValUser);
			JsonObject *pUserJson = static_cast<JsonObject *>(pValUser);
			MUser *user = MUser::CreateFromJsonLPN(*pUserJson);
			AppAssert(user);
			resultObject->GetUsers()->Add(user);
		}

		delete pKeyMessage;
		delete pKeyUsers;
	}
			break;
	case LP_USER_ONLINE:

		jsonArray.GetAt(1, pValUserId);
		pUserId = static_cast<JsonNumber *>(pValUserId);
		resultObject->SetUserId(abs(pUserId->ToInt()));
			break;
	case LP_USER_OFFLINE:
		jsonArray.GetAt(1, pValUserId);
		pUserId = static_cast<JsonNumber *>(pValUserId);
		resultObject->SetUserId(abs(pUserId->ToInt()));

			break;
	case LP_CHAT_UPDATED:

			break;
	case LP_USER_PRINT_MSG:
			jsonArray.GetAt(1, pValUserId);
			pUserId = static_cast<JsonNumber *>(pValUserId);
			resultObject->SetUserId(abs(pUserId->ToInt()));
			break;
	case LP_USER_PRINT_MSG_CHAT:
			jsonArray.GetAt(1, pValUserId);
			pUserId = static_cast<JsonNumber *>(pValUserId);
			resultObject->SetUserId(abs(pUserId->ToInt()));

			jsonArray.GetAt(2, pValChatId);
			pChatId = static_cast<JsonNumber *>(pValChatId);
			resultObject->SetChatId(abs(pChatId->ToInt()));

			break;
	}

	return resultObject;
}

void
LongPollObject::SetUsers(LinkedList * users) {
	__pUsers = users;
}

LinkedList *
LongPollObject::GetUsers() {
	return __pUsers;
}

void
LongPollObject::SetMessage(MMessage * message) {
	__pMessage = message;
}

MMessage *
LongPollObject::GetMessage() {
	return __pMessage;
}

int
LongPollObject::GetMessageId() {
	return __messageId;
}

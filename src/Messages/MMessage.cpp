/*
 * MMessage.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "MMessage.h"
#include "MDialog.h"
#include "MAttachment.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

MMessage::MMessage() {
	uids = null;
	__text = new String(L"");
	__title = null;
	__chatId = 0;
	__pAttachments = null;
}

MMessage::~MMessage() {
	// TODO Auto-generated destructor stub
}

String*
MMessage::TableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS "
			"messages ("
			"_id INTEGER PRIMARY KEY,"
			" mid INTEGER UNIQUE,"
			" uid INTEGER,"
			" from_id INTEGER,"
			" date INTEGER,"
			" out INTEGER,"
			" read_state INTEGER,"
			" text TEXT,"
			" delivered INTEGER"
			")");
	return sql;
}

String*
MMessage::RelationTableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS "
			"m_to_a_relations ("
			"_id INTEGER PRIMARY KEY,"
			"mid INTEGER,"
			"aid INTEGER,"
			" UNIQUE (mid,"
			" aid) "
			")");
	return sql;
}

/***************** GETTERS ******************/

int
MMessage::GetMid() {
	return __mid;
}

int
MMessage::GetUid() {
	return __uid;
}

int
MMessage::GetFromUid() {
	return __fromUid;
}

long
MMessage::GetDate() {
	return __date;
}

int
MMessage::GetOut() {
	return __out;
}
int
MMessage::GetDelivered() {
	return __delivered;
}

int
MMessage::GetReadState() {
	return __readState;
}

String *
MMessage::GetText() {
	return __text;
}

/***************** SETTERS ******************/

void
MMessage::SetMid(int mid) {
	__mid = mid;
}

void
MMessage::SetUid(int uid) {
	__uid = uid;
}

void
MMessage::SetFromUid(int fromUid) {
	__fromUid = fromUid;
}

void
MMessage::SetDate(long date) {
	__date = date;
}

void
MMessage::SetOut(int out) {
	__out = out;
}

void
MMessage::SetDelivered(int delivered) {
	__delivered = delivered;
}

void
MMessage::SetReadState(int state) {
	__readState = state;
}

void
MMessage::SetText(String *text) {
	__text = text;
}

/***************** SETTERS ******************/

MMessage *
MMessage::CreateFromJsonN(const Tizen::Web::Json::JsonObject &pObject) {
	MMessage *message = new (std::nothrow) MMessage();

	JsonString* pKeyId = new JsonString(L"id");
	JsonString* pKeyFromId = new JsonString(L"from_id");
	JsonString* pKeyUserId = new JsonString(L"user_id");
	JsonString* pKeyDate = new JsonString(L"date");
	JsonString* pKeyOut = new JsonString(L"out");
	JsonString* pKeyReadState = new JsonString(L"read_state");
	JsonString* pKeyText = new JsonString(L"body");

	JsonString* pKeyChatId = new JsonString(L"chat_id");
	JsonString* pKeyChatActive = new JsonString(L"chat_active");
	JsonString* pKeyUserCount = new JsonString(L"user_count");
	JsonString* pKeyUsersCount = new JsonString(L"users_count");
	JsonString* pKeyAdminId = new JsonString(L"admin_id");

	IJsonValue* pValMessageId = null;
	IJsonValue* pValFromId = null;
	IJsonValue* pValUserId = null;
	IJsonValue* pValDate = null;
	IJsonValue* pValOut = null;
	IJsonValue* pValReadState = null;
	IJsonValue* pValText = null;

	IJsonValue* pValChatId = null;
	IJsonValue* pValChatActive = null;
	IJsonValue* pValUserCount = null;
	IJsonValue* pValAdminId = null;

	pObject.GetValue(pKeyId, pValMessageId);
	pObject.GetValue(pKeyFromId, pValFromId);
	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyOut, pValOut);
	pObject.GetValue(pKeyReadState, pValReadState);
	pObject.GetValue(pKeyText, pValText);

	pObject.GetValue(pKeyChatId, pValChatId);
	pObject.GetValue(pKeyChatActive, pValChatActive);
	pObject.GetValue(pKeyUserCount, pValUserCount);

	if (!pValUserCount) {
		pObject.GetValue(pKeyUsersCount, pValUserCount);
	}

	pObject.GetValue(pKeyAdminId, pValAdminId);

	JsonNumber* chatId;
	JsonNumber* userCount;
	JsonNumber* adminId;
	JsonArray* uids;

	if (pValChatId) {
		chatId = static_cast< JsonNumber* >(pValChatId);
		message->SetChatId(chatId->ToInt());
	}
	if (pValChatActive && pValUserCount && pValAdminId && pValChatActive) {
		userCount = static_cast< JsonNumber* >(pValUserCount);
		adminId = static_cast< JsonNumber* >(pValAdminId);
		uids = static_cast< JsonArray* >(pValChatActive);

		if (uids) {
			String uidsString(L"");

			for (int index = 0; index < uids->GetCount(); index ++) {
				IJsonValue *pValUserId;
				uids->GetAt(index, pValUserId);
				JsonNumber *userId = static_cast<JsonNumber *>(pValUserId);

				String id;
				id.Format(10, L"%d", userId->ToInt());
				uidsString.Append(id.GetPointer());

				if (index != uids->GetCount() - 1) {
					uidsString.Append(L",");
				}
			}

			message->uids = new String(uidsString);
		}

		message->adminId = adminId->ToInt();
		message->userCount = userCount->ToInt();

	}
	JsonNumber *mid = static_cast< JsonNumber* >(pValMessageId);
	JsonNumber *fromId = static_cast< JsonNumber* >(pValFromId);
	JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
	JsonNumber *date = static_cast< JsonNumber* >(pValDate);
	JsonNumber *out = static_cast< JsonNumber* >(pValOut);
	JsonNumber *readState = static_cast< JsonNumber* >(pValReadState);
	JsonString *text = static_cast< JsonString* >(pValText);

	String *pText = new String(text->GetPointer());

	if (pText) {
		pText->Replace(L"<br>", L"\n");
	}

	message->SetMid(mid->ToInt());
	//костыль для отправки сообщений
	if (fromId) {
		message->SetFromUid(fromId->ToInt());
	} else {
		message->SetFromUid(uid->ToInt());
	}

	if (message->GetChatId() != 0) {
		message->SetUid(message->GetChatId() + 2000000000);
	} else {
		message->SetUid(uid->ToInt());
	}

	message->SetDate(date->ToLong());
	message->SetOut(out->ToInt());
	if (out->ToInt() == 1) {
		message->SetDelivered(1);
	} else {
		message->SetDelivered(0);
	}
	message->SetReadState(readState->ToInt());
	message->SetText(pText);

	JsonString *pKeyAttachments = new JsonString(L"attachments");
	IJsonValue* pValAttachments = null;

	pObject.GetValue(pKeyAttachments, pValAttachments);

	if (pValAttachments) {

		LinkedList *pAttachmentsList = new LinkedList();
		JsonArray *pAttachments = static_cast<JsonArray *>(pValAttachments);

		for (int i = 0; i < pAttachments->GetCount(); i++) {
			IJsonValue* pValAttachment = null;
			pAttachments->GetAt(i, pValAttachment);

			if (pValAttachment) {
				JsonObject *pAttachment = static_cast<JsonObject *>(pValAttachment);

				MAttachment *attachment = MAttachment::CreateFromJsonN(*pAttachment);

				if (attachment) {
					attachment->__mid = mid->ToInt();
					pAttachmentsList->Add(attachment);
				}
			}
		}

		message->__pAttachments = new LinkedList();
		message->__pAttachments->AddItems(*pAttachmentsList);
	}

	delete pKeyAttachments;
	delete pKeyId;
	delete pKeyUserId;
	delete pKeyDate;
	delete pKeyOut;
	delete pKeyReadState;
	delete pKeyFromId;
	delete pKeyText;

	delete pKeyChatId;
	delete pKeyAdminId;
	delete pKeyUserCount;
	delete pKeyUsersCount;
	delete pKeyChatActive;

	return message;
}

MMessage *
MMessage::CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &pObject) {
	MMessage *message = new (std::nothrow) MMessage();

	JsonString* pKeyId = new JsonString(L"mid");
	JsonString* pKeyUserId = new JsonString(L"uid");
	JsonString* pKeyDate = new JsonString(L"date");
	JsonString* pKeyOut = new JsonString(L"out");
	JsonString* pKeyReadState = new JsonString(L"read_state");
	JsonString* pKeyText = new JsonString(L"body");

	JsonString* pKeyChatId = new JsonString(L"chat_id");
	JsonString* pKeyChatActive = new JsonString(L"chat_active");
	JsonString* pKeyUserCount = new JsonString(L"users_count");
	JsonString* pKeyAdminId = new JsonString(L"admin_id");
	JsonString* pKeyTitle = new JsonString(L"title");

	IJsonValue* pValMessageId = null;
	IJsonValue* pValUserId = null;
	IJsonValue* pValDate = null;
	IJsonValue* pValOut = null;
	IJsonValue* pValReadState = null;
	IJsonValue* pValText = null;

	IJsonValue* pValChatId = null;
	IJsonValue* pValChatActive = null;
	IJsonValue* pValUserCount = null;
	IJsonValue* pValAdminId = null;
	IJsonValue* pValTitleId = null;

	pObject.GetValue(pKeyId, pValMessageId);
	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyOut, pValOut);
	pObject.GetValue(pKeyReadState, pValReadState);
	pObject.GetValue(pKeyText, pValText);

	pObject.GetValue(pKeyChatId, pValChatId);
	pObject.GetValue(pKeyChatActive, pValChatActive);
	pObject.GetValue(pKeyUserCount, pValUserCount);
	pObject.GetValue(pKeyAdminId, pValAdminId);
	pObject.GetValue(pKeyTitle, pValTitleId);

	JsonNumber *mid = static_cast< JsonNumber* >(pValMessageId);
	JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
	JsonNumber *date = static_cast< JsonNumber* >(pValDate);
	JsonNumber *out = static_cast< JsonNumber* >(pValOut);
	JsonNumber *readState = static_cast< JsonNumber* >(pValReadState);
	JsonString *text = static_cast< JsonString* >(pValText);

	JsonNumber* chatId;
	JsonNumber* userCount;
	JsonNumber* adminId;
	JsonString* uids;
	JsonString* title;

	if (pValChatId) {
		chatId = static_cast< JsonNumber* >(pValChatId);
		message->SetChatId(chatId->ToInt());
	}

	if (pValUserCount) {
		userCount = static_cast< JsonNumber* >(pValUserCount);
		adminId = static_cast< JsonNumber* >(pValAdminId);
		uids = static_cast< JsonString* >(pValChatActive);
		title = static_cast< JsonString* >(pValTitleId);

		message->uids = new String(uids->GetPointer());

		message->adminId = adminId->ToInt();

		message->userCount = userCount->ToInt();
		message->__title = new String(title->GetPointer());
	}


	String *pText = new String(text->GetPointer());

	if (pText) {
		pText->Replace(L"<br>", L"\n");
	}

	message->SetMid(mid->ToInt());

	if (message->GetChatId() != 0) {
		AppLog("1111trttttttttt");
		message->SetUid(message->GetChatId() + isChatValue);
	} else {
		message->SetUid(uid->ToInt());
	}

	message->SetFromUid(uid->ToInt());
	message->SetDate(date->ToLong());
	message->SetOut(out->ToInt());

	if (out->ToInt() == 1) {
		message->SetDelivered(1);
	} else {
		message->SetDelivered(0);
	}
	message->SetReadState(readState->ToInt());
	message->SetText(pText);

	delete pKeyId;
	delete pKeyUserId;
	delete pKeyDate;
	delete pKeyOut;
	delete pKeyReadState;
	delete pKeyText;
	delete pKeyTitle;

	delete pKeyChatId;
	delete pKeyAdminId;
	delete pKeyUserCount;
	delete pKeyChatActive;

	JsonString *pKeyAttachments = new JsonString(L"attachments");
	IJsonValue* pValAttachments = null;

	pObject.GetValue(pKeyAttachments, pValAttachments);

	if (pValAttachments) {

		LinkedList *pAttachmentsList = new LinkedList();
		JsonArray *pAttachments = static_cast<JsonArray *>(pValAttachments);

		for (int i = 0; i < pAttachments->GetCount(); i++) {
			IJsonValue* pValAttachment = null;
			pAttachments->GetAt(i, pValAttachment);

			if (pValAttachment) {
				JsonObject *pAttachment = static_cast<JsonObject *>(pValAttachment);

				MAttachment *attachment = MAttachment::CreateFromJsonLPN(*pAttachment);

				if (attachment) {
					attachment->__mid = mid->ToInt();
					pAttachmentsList->Add(attachment);
				}
			}
		}

		message->__pAttachments = new LinkedList();
		message->__pAttachments->AddItems(*pAttachmentsList);
	}

	AppLog("1111test");

	return message;
}

int
MMessage::GetChatId() {
	return __chatId;
}

void
MMessage::SetChatId(int chatId) {
	__chatId = chatId;
}

/*
 * MMessage.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "MMessage.h"
#include "MDialog.h"
#include "MAttachment.h"
#include "MGeo.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Web::Json;

MMessage::MMessage() {
	uids = null;
	__text = null;
	__title = null;
	__chatId = 0;
	__pAttachments = null;
	__pFwd = null;
	__pGeo = null;
	__owner = 0;
	__pUser = null;
//	__text = new String(L"");
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
MMessage::FwdTableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS "
			"fwd_messages ("
			" mid INTEGER PRIMARY KEY,"
			" uid INTEGER,"
			" date INTEGER,"
			" text TEXT, "
			" owner INTEGER"
			")");
	return sql;
}

String*
MMessage::FwdRelationTableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS "
			"m_to_fm_relations ("
			"_id INTEGER PRIMARY KEY,"
			"fmid INTEGER,"
			"aid INTEGER,"
			" UNIQUE (fmid,"
			" aid) "
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
	if (__text) {
		delete __text;
		__text = null;
	}
	__text = text;
}

/***************** SETTERS ******************/

MMessage *
MMessage::CreateFromJsonN(const Tizen::Web::Json::JsonObject &pObject, bool fwd) {
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

	if (!fwd) {
		pObject.GetValue(pKeyId, pValMessageId);
		pObject.GetValue(pKeyFromId, pValFromId);
		pObject.GetValue(pKeyOut, pValOut);
		pObject.GetValue(pKeyReadState, pValReadState);
	}

	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyText, pValText);

	if (!fwd) {
		pObject.GetValue(pKeyChatId, pValChatId);
		pObject.GetValue(pKeyChatActive, pValChatActive);
		pObject.GetValue(pKeyUserCount, pValUserCount);

		if (!pValUserCount) {
			pObject.GetValue(pKeyUsersCount, pValUserCount);
		}

		pObject.GetValue(pKeyAdminId, pValAdminId);
	}

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

	JsonNumber *mid = null;
	JsonNumber *fromId = null;
	JsonNumber *uid = null;
	JsonNumber *date = null;
	JsonNumber *out = null;
	JsonNumber *readState = null;
	JsonString *text = null;

	uid = static_cast< JsonNumber* >(pValUserId);
	date = static_cast< JsonNumber* >(pValDate);
	text = static_cast< JsonString* >(pValText);

	if (!fwd) {
		mid = static_cast< JsonNumber* >(pValMessageId);
		fromId = static_cast< JsonNumber* >(pValFromId);
		out = static_cast< JsonNumber* >(pValOut);
		readState = static_cast< JsonNumber* >(pValReadState);
	}

	String *pText = new String(text->GetPointer());

	if (pText && pText->GetLength() > 0) {
		pText->Replace(L"<br>", L"\n");
	}

	if (!fwd) {
		message->SetMid(mid->ToInt());
		//костыль для отправки сообщений
		if (fromId) {
			message->SetFromUid(fromId->ToInt());
		} else {
			message->SetFromUid(uid->ToInt());
		}
	}

	if (message->GetChatId() != 0) {
		message->SetUid(message->GetChatId() + 2000000000);
	} else {
		message->SetUid(uid->ToInt());
	}

	message->SetDate(date->ToLong());
	message->SetText(pText);

	if (!fwd) {
		message->SetOut(out->ToInt());
		if (out->ToInt() == 1) {
			message->SetDelivered(1);
		} else {
			message->SetDelivered(0);
		}
		message->SetReadState(readState->ToInt());
	}

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
					if (mid) {
						attachment->__mid = mid->ToInt();
					}
					pAttachmentsList->Add(attachment);
				}
			}
		}

		message->__pAttachments = new LinkedList();
		message->__pAttachments->AddItems(*pAttachmentsList);
	}

	JsonString* pKeyGeo = new JsonString(L"geo");
	IJsonValue* pValGeo = null;

	pObject.GetValue(pKeyGeo, pValGeo);

	if (pValGeo) {
		JsonObject *pGaoObject = static_cast<JsonObject*>(pValGeo);
		MGeo *pGeo = MGeo::CreateFromJsonN(*pGaoObject);
		if (mid) {
			pGeo->__mid = mid->ToInt();
		}

		if (pGeo) {
			message->__pGeo = pGeo;
		}
	}

	delete pKeyGeo;

	if (fwd) {
		message->__pType = new String(L"fwd");
	}

	JsonString* pKeyFwdMessages = new JsonString(L"fwd_messages");
	IJsonValue* pValFwds = null;

	pObject.GetValue(pKeyFwdMessages, pValFwds);

	if (pValFwds) {

		AppLog("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

		JsonArray *pFwds = static_cast<JsonArray *>(pValFwds);
		for (int i = 0; i < pFwds->GetCount(); i++) {
			IJsonValue* pValFwd = null;
			pFwds->GetAt(i, pValFwd);

			JsonObject *pFwdObject = static_cast<JsonObject *>(pValFwd);

			MMessage *pFwdMessage = MMessage::CreateFromJsonN(*pFwdObject, true);

			if (!message->__pFwd) {
				message->__pFwd = new LinkedList();
			}

			message->__pFwd->Add(pFwdMessage);
		}
	}

	delete pKeyFwdMessages;


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
MMessage::CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &pObject, bool fwd) {
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

	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyText, pValText);

	if (!fwd) {
		pObject.GetValue(pKeyId, pValMessageId);

		pObject.GetValue(pKeyOut, pValOut);
		pObject.GetValue(pKeyReadState, pValReadState);

		pObject.GetValue(pKeyChatId, pValChatId);
		pObject.GetValue(pKeyChatActive, pValChatActive);
		pObject.GetValue(pKeyUserCount, pValUserCount);
		pObject.GetValue(pKeyAdminId, pValAdminId);
		pObject.GetValue(pKeyTitle, pValTitleId);
	}

	JsonNumber *mid = null;
	JsonNumber *uid = null;
	JsonNumber *date = null;
	JsonNumber *out = null;
	JsonNumber *readState = null;
	JsonString *text = null;

	JsonNumber* chatId;
	JsonNumber* userCount;
	JsonNumber* adminId;
	JsonString* uids;
	JsonString* title;


	uid = static_cast< JsonNumber* >(pValUserId);
	date = static_cast< JsonNumber* >(pValDate);
	text = static_cast< JsonString* >(pValText);

	if (!fwd) {
		mid = static_cast< JsonNumber* >(pValMessageId);

		out = static_cast< JsonNumber* >(pValOut);
		readState = static_cast< JsonNumber* >(pValReadState);

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
	}

	String *pText = new String(text->GetPointer());

	if (pText && pText->GetLength() > 0) {
		pText->Replace(L"<br>", L"\n");
	}

	if (!fwd) {
		message->SetMid(mid->ToInt());
		message->SetFromUid(uid->ToInt());
		message->SetOut(out->ToInt());

		if (out->ToInt() == 1) {
			message->SetDelivered(1);
		} else {
			message->SetDelivered(0);
		}
		message->SetReadState(readState->ToInt());
	}

	if (message->GetChatId() != 0) {
		AppLog("1111trttttttttt");
		message->SetUid(message->GetChatId() + isChatValue);
	} else {
		message->SetUid(uid->ToInt());
	}

	message->SetDate(date->ToLong());
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
					if (mid) {
						attachment->__mid = mid->ToInt();
					}
					pAttachmentsList->Add(attachment);
				}
			}
		}

		message->__pAttachments = new LinkedList();
		message->__pAttachments->AddItems(*pAttachmentsList);
	}

	delete pKeyAttachments;

	JsonString* pKeyGeo = new JsonString(L"geo");
	IJsonValue* pValGeo = null;

	pObject.GetValue(pKeyGeo, pValGeo);

	if (pValGeo) {
		JsonObject *pGaoObject = static_cast<JsonObject*>(pValGeo);
		MGeo *pGeo = MGeo::CreateFromJsonN(*pGaoObject);
		if (mid) {
			pGeo->__mid = mid->ToInt();
		}
		if (pGeo) {
			message->__pGeo = pGeo;
		}
	}

	delete pKeyGeo;

	if (fwd) {
		message->__pType = new String(L"fwd");
	}

	JsonString* pKeyFwdMessages = new JsonString(L"fwd_messages");
	IJsonValue* pValFwds = null;

	pObject.GetValue(pKeyFwdMessages, pValFwds);

	if (pValFwds) {
		JsonArray *pFwds = static_cast<JsonArray *>(pValFwds);
		for (int i = 0; i < pFwds->GetCount(); i++) {
			IJsonValue* pValFwd = null;
			pFwds->GetAt(i, pValFwd);

			JsonObject *pFwdObject = static_cast<JsonObject *>(pValFwd);

			MMessage *pFwdMessage = MMessage::CreateFromJsonLPN(*pFwdObject, true);

			if (!message->__pFwd) {
				message->__pFwd = new LinkedList();
			}

			message->__pFwd->Add(pFwdMessage);
		}
	}

	delete pKeyFwdMessages;

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

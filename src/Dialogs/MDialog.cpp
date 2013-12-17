/*
 * MDialog.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "MDialog.h"
#include "MUser.h"
#include "Helper.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

const int isChatValue = 2000000000;

MDialog::MDialog() {
	__chatId = 0;

	__pUsers = null;
	chatUids = null;

	__firstName = null;
	__lastName = null;
	__photo = null;
	__miniPhoto = null;

    __title = null;
	__text = null;

	__pUser = null;
	__pType = null;
}

MDialog::~MDialog() {
	SAFE_DELETE(chatUids);
	SAFE_DELETE(__firstName);
	SAFE_DELETE(__lastName);
	SAFE_DELETE(__photo);
	SAFE_DELETE(__miniPhoto);
	SAFE_DELETE(__title);
	SAFE_DELETE(__text);
	SAFE_DELETE(__pUser);
	SAFE_DELETE(__pType);

//	if (__pUsers) {
//		__pUsers->RemoveAll(true);
//		delete __pUsers;
//		__pUsers = null;
//	}
}

String*
MDialog::TableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS dialogs (_id INTEGER PRIMARY KEY, identifier INTEGER, uid INTEGER UNIQUE, last_name TEXT, first_name TEXT, photo TEXT, mini_photo TEXT, is_online INTEGER, date INTEGER, out INTEGER, read_state INTEGER, title TEXT, text TEXT, chat_id INTEGER, chat_uids TEXT)");
	return sql;
}

/***************** GETTERS ******************/

String *
MDialog::GetFirstName() {
	return __firstName;
}

String *
MDialog::GetLastName() {
	return __lastName;
}

String *
MDialog::GetPhoto() {
	return __photo;
}

String *
MDialog::GetMiniPhoto() {
	return __miniPhoto;
}

int
MDialog::GetUid() {
	return __uid;
}

int
MDialog::GetIsOnline() {
	return __isOnline;
}

int
MDialog::GetIdentifier() {
	return __identifier;
}

long
MDialog::GetDate() {
	return __date;
}

int
MDialog::GetOut() {
	return __out;
}

int
MDialog::GetReadState() {
	return __readState;
}

String *
MDialog::GetTitle() {
	return __title;
}

String *
MDialog::GetText() {
	return __text;
}

int
MDialog::GetChatId() {
	return __chatId;
}

String *
MDialog::GetChatUids() {
	return chatUids;
}

ArrayList*
MDialog::GetChatUidsArray() {
	return new ArrayList();
}

LinkedList *
MDialog::GetUsers() {
	return __pUsers;
}

/***************** SETTERS ******************/


void
MDialog::SetUsers(LinkedList *list) {
	__pUsers = list;
}

void
MDialog::SetFirstName(String *firstName) {
	__firstName = firstName;
}

void
MDialog::SetLastName(String *lastName) {
	__lastName = lastName;
}

void
MDialog::SetPhoto(String *photo) {
	__photo = photo;
}

void
MDialog::SetMiniPhoto(String *miniPhoto) {
	__miniPhoto = miniPhoto;
}

void
MDialog::SetUid(int uid) {
	__uid = uid;
}

void
MDialog::SetIsOnline(int isOnline) {
	__isOnline = isOnline;
}

void
MDialog::SetIdentifier(int id) {
	__identifier = id;
}

void
MDialog::SetDate(long date) {
	__date = date;
}

void
MDialog::SetOut(int out) {
	__out = out;
}

void
MDialog::SetReadState(int state) {
	__readState = state;
}

void
MDialog::SetTitle(String *title) {
	__title = title;
}

void
MDialog::SetText(String *text) {
	__text = text;
}

void
MDialog::SetChatId(int id) {
	__chatId = id;
}

void
MDialog::SetChatUids(String *uids) {
	chatUids = uids;
}

/***************** SETTERS ******************/

MDialog *
MDialog::CreateFromJsonN(
		const Tizen::Web::Json::JsonObject &pUserObject,
		const Tizen::Web::Json::JsonObject &pMessageObject)
{
	MDialog *dialog = new (std::nothrow) MDialog();

	//common
	JsonString* pKeyId = new JsonString(L"id");

	//user
	JsonString* pKeyFirstName = new JsonString(L"first_name");
	JsonString* pKeyLastName = new JsonString(L"last_name");
	JsonString* pKeyMiniPhoto = new JsonString(L"photo_50");
	JsonString* pKeyPhoto = new JsonString(L"photo_100");
	JsonString* pKeyOnline = new JsonString(L"online");

	//message
	JsonString* pKeyUserId = new JsonString(L"user_id");
	JsonString* pKeyDate = new JsonString(L"date");
	JsonString* pKeyOut = new JsonString(L"out");
	JsonString* pKeyReadState = new JsonString(L"read_state");
	JsonString* pKeyTitle = new JsonString(L"title");
	JsonString* pKeyText = new JsonString(L"body");

	JsonString* pKeyChatId = new JsonString(L"chat_id");
	JsonString* pKeyChatActive = new JsonString(L"chat_active");
	JsonString* pKeyUserCount = new JsonString(L"user_count");
	JsonString* pKeyAdminId = new JsonString(L"admin_id");

	//user
	IJsonValue* pValUserId = null;
	IJsonValue* pValFirstName = null;
	IJsonValue* pValLastName = null;
	IJsonValue* pValMiniPhoto = null;
	IJsonValue* pValPhoto = null;
	IJsonValue* pValOnline = null;

	//message
	IJsonValue* pValMessageId = null;
	IJsonValue* pValMessageUserId = null;
	IJsonValue* pValDate = null;
	IJsonValue* pValOut = null;
	IJsonValue* pValReadState = null;
	IJsonValue* pValTitle = null;
	IJsonValue* pValText = null;
	IJsonValue* pValChatId = null;
	IJsonValue* pValChatActive = null;
	IJsonValue* pValUserCount = null;
	IJsonValue* pValAdminId = null;


	//user
	pUserObject.GetValue(pKeyId, pValUserId);
	pUserObject.GetValue(pKeyFirstName, pValFirstName);
	pUserObject.GetValue(pKeyLastName, pValLastName);
	pUserObject.GetValue(pKeyMiniPhoto, pValMiniPhoto);
	pUserObject.GetValue(pKeyPhoto, pValPhoto);
	pUserObject.GetValue(pKeyOnline, pValOnline);

	JsonString *firstName = static_cast< JsonString* >(pValFirstName);
	JsonString *lastName = static_cast< JsonString* >(pValLastName);
//	JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
	JsonString *miniPhoto = static_cast< JsonString* >(pValMiniPhoto);
	JsonString *photo = static_cast< JsonString* >(pValPhoto);
	JsonNumber *isOnline = static_cast< JsonNumber* >(pValOnline);

	//message
	pMessageObject.GetValue(pKeyId, pValMessageId);
	pMessageObject.GetValue(pKeyUserId, pValMessageUserId);
	pMessageObject.GetValue(pKeyDate, pValDate);
	pMessageObject.GetValue(pKeyOut, pValOut);
	pMessageObject.GetValue(pKeyReadState, pValReadState);
	pMessageObject.GetValue(pKeyTitle, pValTitle);
	pMessageObject.GetValue(pKeyText, pValText);
	pMessageObject.GetValue(pKeyChatId, pValChatId);

	pMessageObject.GetValue(pKeyChatActive, pValChatActive);
	pMessageObject.GetValue(pKeyUserCount, pValUserCount);
	pMessageObject.GetValue(pKeyAdminId, pValAdminId);

	JsonNumber *mid = static_cast< JsonNumber* >(pValMessageId);
	JsonNumber *muid = null;
	JsonNumber *chatId = null;

	if (pValChatId) {
		muid = static_cast< JsonNumber* >(pValChatId);
		chatId = static_cast< JsonNumber* >(pValChatId);
	} else {
		muid = static_cast< JsonNumber* >(pValMessageUserId);
	}

	JsonNumber *date = static_cast< JsonNumber* >(pValDate);
	JsonNumber *out = static_cast< JsonNumber* >(pValOut);
	JsonNumber *readState = static_cast< JsonNumber* >(pValReadState);
	JsonString *title = static_cast< JsonString* >(pValTitle);
	JsonString *text = static_cast< JsonString* >(pValText);

	//values
	String *pFirstName = new String(firstName->GetPointer());
	String *pLastName = new String(lastName->GetPointer());
	String *pMiniPhoto = new String(miniPhoto->GetPointer());
	String *pPhoto = new String(photo->GetPointer());

	String *pTitle = new String(title->GetPointer());
	String *pText = new String(text->GetPointer());

	dialog->SetIdentifier(mid->ToInt());

	if (chatId) {
		dialog->SetUid(chatId->ToInt() + 2000000000);
		dialog->SetChatId(chatId->ToInt());
	} else {
		dialog->SetUid(muid->ToInt());
	}

	if (pValChatActive) {
		String uids(L"");

		JsonArray *pArrayObject = static_cast<JsonArray *>(pValChatActive);

		for (int index = 0; index < pArrayObject->GetCount(); index ++) {
			IJsonValue *pValUserId;
			pArrayObject->GetAt(index, pValUserId);
			JsonNumber *userId = static_cast<JsonNumber *>(pValUserId);

			String id;
			id.Format(10, L"%d", userId->ToInt());
			uids.Append(id.GetPointer());

			if (index != pArrayObject->GetCount() - 1) {
				uids.Append(L",");
			}
		}

		dialog->SetChatUids(new String(uids));
	}

	dialog->SetFirstName(pFirstName);
	dialog->SetLastName(pLastName);
	dialog->SetMiniPhoto(pMiniPhoto);
	dialog->SetPhoto(pPhoto);
	dialog->SetIsOnline(isOnline->ToInt());

	dialog->SetDate(date->ToLong());
	dialog->SetOut(out->ToInt());
	dialog->SetReadState(readState->ToInt());
	dialog->SetTitle(pTitle);
	dialog->SetText(pText);

	delete pKeyId;

	delete pKeyFirstName;
	delete pKeyLastName;
	delete pKeyMiniPhoto;
	delete pKeyPhoto;
	delete pKeyOnline;

	delete pKeyUserId;
	delete pKeyDate;
	delete pKeyOut;
	delete pKeyReadState;
	delete pKeyTitle;
	delete pKeyText;
	delete pKeyChatId;

	delete pKeyAdminId;
	delete pKeyUserCount;
	delete pKeyChatActive;

	return dialog;
}

MDialog *
MDialog::CreateSearchDialogFromJsonN(const Tizen::Web::Json::JsonObject &pObject) {
	MDialog *dialog = new (std::nothrow) MDialog();

	//common
	JsonString* pKeyId = new JsonString(L"id");

	//user
	JsonString* pKeyFirstName = new JsonString(L"first_name");
	JsonString* pKeyLastName = new JsonString(L"last_name");
	JsonString* pKeyMiniPhoto = new JsonString(L"photo_50");
	JsonString* pKeyPhoto = new JsonString(L"photo_100");
	JsonString* pKeyOnline = new JsonString(L"online");

	JsonString* pKeyUserCount = new JsonString(L"users");
	JsonString* pKeyAdminId = new JsonString(L"admin_id");

	//
	JsonString* pKeyTitle = new JsonString(L"title");
	JsonString* pKeyType = new JsonString(L"type");

	IJsonValue* pValType = null;
	pObject.GetValue(pKeyType, pValType);
	JsonString *pTypeString = static_cast< JsonString* >(pValType);
	String *pType = new String(pTypeString->GetPointer());

	IJsonValue* pValUserId = null;
	pObject.GetValue(pKeyId, pValUserId);
	JsonNumber *userId = static_cast<JsonNumber *>(pValUserId);

	int id = userId->ToInt();

	if (pType->Equals(L"chat", false)) {
		IJsonValue* pValUsers = null;
		IJsonValue* pValAdminId = null;

		IJsonValue* pValTitle = null;
		pObject.GetValue(pKeyTitle, pValTitle);
		pObject.GetValue(pKeyUserCount, pValUsers);
		pObject.GetValue(pKeyAdminId, pValAdminId);

		if (pValUsers) {
			String uids(L"");

			JsonArray *pArrayObject = static_cast<JsonArray *>(pValUsers);

			for (int index = 0; index < pArrayObject->GetCount(); index ++) {
				IJsonValue *pValUserId;
				pArrayObject->GetAt(index, pValUserId);
				JsonNumber *userId = static_cast<JsonNumber *>(pValUserId);

				String id;
				id.Format(10, L"%d", userId->ToInt());
				uids.Append(id.GetPointer());

				if (index != pArrayObject->GetCount() - 1) {
					uids.Append(L",");
				}
			}

			dialog->SetChatUids(new String(uids));
		}

		JsonString *title = static_cast< JsonString* >(pValTitle);
		String *pTitle = new String(title->GetPointer());

		dialog->SetChatId(id);
		dialog->SetUid(id + isChatValue);
		if (pTitle) {
			dialog->SetTitle(new String(pTitle->GetPointer()));
		}
		if (pTitle) {
			dialog->SetFirstName(pTitle);
		}
		dialog->SetLastName(new String(L""));
	} else {
	//user

		IJsonValue* pValFirstName = null;
		IJsonValue* pValLastName = null;
		IJsonValue* pValMiniPhoto = null;
		IJsonValue* pValPhoto = null;
		IJsonValue* pValOnline = null;

	//user

		pObject.GetValue(pKeyFirstName, pValFirstName);
		pObject.GetValue(pKeyLastName, pValLastName);
		pObject.GetValue(pKeyMiniPhoto, pValMiniPhoto);
		pObject.GetValue(pKeyPhoto, pValPhoto);
		pObject.GetValue(pKeyOnline, pValOnline);

		JsonString *firstName = static_cast< JsonString* >(pValFirstName);
		JsonString *lastName = static_cast< JsonString* >(pValLastName);
		JsonString *miniPhoto = static_cast< JsonString* >(pValMiniPhoto);
		JsonString *photo = static_cast< JsonString* >(pValPhoto);
		JsonNumber *isOnline = static_cast< JsonNumber* >(pValOnline);

		String *pFirstName = new String(firstName->GetPointer());
		String *pLastName = new String(lastName->GetPointer());
		String *pMiniPhoto = new String(miniPhoto->GetPointer());
		String *pPhoto = new String(photo->GetPointer());

		dialog->SetFirstName(pFirstName);
		dialog->SetLastName(pLastName);
		dialog->SetMiniPhoto(pMiniPhoto);
		dialog->SetPhoto(pPhoto);
		dialog->SetIsOnline(isOnline->ToInt());

		dialog->SetUid(id);
	}

	dialog->__pType = pType;

	delete pKeyId;

	delete pKeyFirstName;
	delete pKeyLastName;
	delete pKeyMiniPhoto;
	delete pKeyPhoto;
	delete pKeyOnline;

	delete pKeyTitle;

	delete pKeyAdminId;
	delete pKeyUserCount;
	delete pKeyType;

	return dialog;

}

MDialog *
MDialog::CreateFromUserN(MUser *pUser) {
	MDialog *dialog = new MDialog();
	dialog->SetIsOnline(pUser->GetIsOnline());
	dialog->SetUid(pUser->GetUid());

	if (pUser->GetFirstName()) {
		dialog->SetFirstName(new String(pUser->GetFirstName()->GetPointer()));
	}

	if (pUser->GetLastName()) {
		dialog->SetLastName(new String(pUser->GetLastName()->GetPointer()));
	}

	if (pUser->GetMiniPhoto()) {
		dialog->SetMiniPhoto(new String(pUser->GetMiniPhoto()->GetPointer()));
	}

	if (pUser->GetPhoto()) {
		dialog->SetPhoto(new String(pUser->GetPhoto()->GetPointer()));
	}

	return dialog;
}


MUser *
MDialog::GetUser() {
	if (!__pUser) {
		MUser *pUser = new MUser();
		if (this->GetFirstName()) {
			pUser->SetFirstName(new String(this->GetFirstName()->GetPointer()));
		}

		if (this->GetLastName()) {
			pUser->SetLastName(new String(this->GetLastName()->GetPointer()));
		}

		pUser->SetIsOnline(this->GetIsOnline());

		if (this->GetMiniPhoto()) {
			pUser->SetMiniPhoto(new String(this->GetMiniPhoto()->GetPointer()));
		}

		if (this->GetPhoto()) {
			pUser->SetPhoto(new String(this->GetPhoto()->GetPointer()));
		}

		pUser->SetUid(this->GetUid());

		__pUser = pUser;
	}

	return __pUser;
}

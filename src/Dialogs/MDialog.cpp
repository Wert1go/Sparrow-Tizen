/*
 * MDialog.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "MDialog.h"
#include "MUser.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

const int isChatValue = 2000000000;

MDialog::MDialog() {
	__pUsers = null;
	chatUids = null;
	__chatId = 0;
}

MDialog::~MDialog() {
	// TODO Auto-generated destructor stub
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

		AppLog("pValChatActive pValChatActive pValChatActive pValChatActive");

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

		AppLog("uids: %S", uids.GetPointer());

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
MDialog::CreateFromUserN(MUser *pUser) {
	MDialog *dialog = new MDialog();
	dialog->SetFirstName(pUser->GetFirstName());
	dialog->SetLastName(pUser->GetLastName());
	dialog->SetIsOnline(pUser->GetIsOnline());
	dialog->SetMiniPhoto(pUser->GetMiniPhoto());
	dialog->SetPhoto(pUser->GetPhoto());
	dialog->SetUid(pUser->GetUid());

	return dialog;
}

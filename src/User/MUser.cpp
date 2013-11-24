	/*
 * User.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#include "MUser.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

MUser::MUser() {
	__isFriend = 0;
	__isContact = 0;
	__isPending = 0;
}

MUser::~MUser() {
	delete __firstName;
	delete __lastName;
}

/***************** GETTERS ******************/
String *
MUser::GetFirstName() {
	return __firstName;
}

String *
MUser::GetLastName() {
	return __lastName;
}

String *
MUser::GetPhoto() {
	return __photo;
}

String *
MUser::GetMiniPhoto() {
	return __miniPhoto;
}

int
MUser::GetUid() {
	return __uid;
}

long
MUser::GetLastSeen() {
	return __lastSeen;
}

int
MUser::GetIsOnline() {
	return __isOnline;
}

int MUser::GetType() {
	return __type;
}



/***************** SETTERS ******************/

void MUser::SetFirstName(String *firstName) {
	__firstName = firstName;
}

void MUser::SetLastName(String *lastName) {
	__lastName = lastName;
}

void MUser::SetPhoto(String *photo) {
	__photo = photo;
}

void MUser::SetMiniPhoto(String *miniPhoto) {
	__miniPhoto = miniPhoto;
}

void MUser::SetUid(int uid) {
	__uid = uid;
}

void MUser::SetLastSeen(long lastSeen) {
	__lastSeen = lastSeen;
}

void MUser::SetIsOnline(int isOnline) {
	__isOnline = isOnline;
}

void MUser::SetType(int type) {
	__type = type;
}



/***************** SETTERS ******************/

MUser *
MUser::CreateFromJsonN(const Tizen::Web::Json::JsonObject &pUserObject) {

	MUser *user = new (std::nothrow) MUser();

	JsonString* pKeyFirstName = new JsonString(L"first_name");
	JsonString* pKeyLastName = new JsonString(L"last_name");
	JsonString* pKeyId = new JsonString(L"id");
	JsonString* pKeyMiniPhoto = new JsonString(L"photo_50");
	JsonString* pKeyPhoto = new JsonString(L"photo_100");
	JsonString* pKeyOnline = new JsonString(L"online");
	JsonString* pKeyLastSeen = new JsonString(L"last_seen");
	JsonString* pKeyTime = new JsonString(L"time");
	JsonString* pKeyIsFriend = new JsonString(L"is_friend");

	IJsonValue* pValFirstName = null;
	IJsonValue* pValLastName = null;
	IJsonValue* pValId = null;
	IJsonValue* pValMiniPhoto = null;
	IJsonValue* pValPhoto = null;
	IJsonValue* pValOnline = null;
	IJsonValue* pValLastSeen = null;
	IJsonValue* pValTime = null;
	IJsonValue* pValIsFriend = null;

	pUserObject.GetValue(pKeyFirstName, pValFirstName);
	pUserObject.GetValue(pKeyLastName, pValLastName);
	pUserObject.GetValue(pKeyId, pValId);
	pUserObject.GetValue(pKeyMiniPhoto, pValMiniPhoto);
	pUserObject.GetValue(pKeyPhoto, pValPhoto);
	pUserObject.GetValue(pKeyOnline, pValOnline);
	pUserObject.GetValue(pKeyLastSeen, pValLastSeen);
	pUserObject.GetValue(pKeyIsFriend, pValIsFriend);

	JsonString *firstName = static_cast< JsonString* >(pValFirstName);
	JsonString *lastName = static_cast< JsonString* >(pValLastName);
	JsonNumber *uid = static_cast< JsonNumber* >(pValId);
	JsonString *miniPhoto = static_cast< JsonString* >(pValMiniPhoto);
	JsonString *photo = static_cast< JsonString* >(pValPhoto);
	JsonNumber *isOnline = static_cast< JsonNumber* >(pValOnline);
	JsonNumber *isFriend = static_cast< JsonNumber* >(pValIsFriend);

	JsonNumber *time = null;
	if (pValLastSeen) {
		JsonObject *lastSeen = static_cast< JsonObject* >(pValLastSeen);
		lastSeen->GetValue(pKeyTime, pValTime);
		time = static_cast< JsonNumber* >(pValTime);
	}

	String *pFirstName = new String(firstName->GetPointer());
	String *pLastName = new String(lastName->GetPointer());
	String *pMiniPhoto = new String(miniPhoto->GetPointer());
	String *pPhoto = new String(photo->GetPointer());

	user->SetFirstName(pFirstName);
	user->SetLastName(pLastName);
	user->SetMiniPhoto(pMiniPhoto);
	user->SetPhoto(pPhoto);
	user->SetUid(uid->ToInt());
	user->SetIsOnline(isOnline->ToInt());

	if (isFriend) {
		user->__isFriend = isFriend->ToInt();
	} else {
		AppLog("user->__isFriend = 0;");
		user->__isFriend = 0;
	}

	if (time) {
		user->SetLastSeen(time->ToLong());
	}

	delete pKeyFirstName;
	delete pKeyLastName;
	delete pKeyId;
	delete pKeyMiniPhoto;
	delete pKeyPhoto;
	delete pKeyOnline;
	delete pKeyLastSeen;
	delete pKeyTime;
	delete pKeyIsFriend;

	return user;
}

//ужс :)
MUser *
MUser::CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &pUserObject) {

	MUser *user = new (std::nothrow) MUser();

	JsonString* pKeyFirstName = new JsonString(L"first_name");
	JsonString* pKeyLastName = new JsonString(L"last_name");
	JsonString* pKeyId = new JsonString(L"uid");
	JsonString* pKeyMiniPhoto = new JsonString(L"photo");
	JsonString* pKeyPhoto = new JsonString(L"photo_medium_rec");
	JsonString* pKeyOnline = new JsonString(L"online");
	JsonString* pKeyLastSeen = new JsonString(L"last_seen");
	JsonString* pKeyTime = new JsonString(L"time");
	JsonString* pKeyIsFriend = new JsonString(L"is_friend");

	IJsonValue* pValFirstName = null;
	IJsonValue* pValLastName = null;
	IJsonValue* pValId = null;
	IJsonValue* pValMiniPhoto = null;
	IJsonValue* pValPhoto = null;
	IJsonValue* pValOnline = null;
	IJsonValue* pValLastSeen = null;
	IJsonValue* pValTime = null;
	IJsonValue* pValIsFriend = null;

	pUserObject.GetValue(pKeyFirstName, pValFirstName);
	pUserObject.GetValue(pKeyLastName, pValLastName);
	pUserObject.GetValue(pKeyId, pValId);
	pUserObject.GetValue(pKeyMiniPhoto, pValMiniPhoto);
	pUserObject.GetValue(pKeyPhoto, pValPhoto);
	pUserObject.GetValue(pKeyOnline, pValOnline);
	pUserObject.GetValue(pKeyLastSeen, pValLastSeen);
	pUserObject.GetValue(pKeyIsFriend, pValIsFriend);

	JsonString *firstName = static_cast< JsonString* >(pValFirstName);
	JsonString *lastName = static_cast< JsonString* >(pValLastName);
	JsonNumber *uid = static_cast< JsonNumber* >(pValId);
	JsonString *miniPhoto = static_cast< JsonString* >(pValMiniPhoto);
	JsonString *photo = static_cast< JsonString* >(pValPhoto);
	JsonNumber *isOnline = static_cast< JsonNumber* >(pValOnline);
	JsonNumber *isFriend = static_cast< JsonNumber* >(pValIsFriend);

	JsonObject *lastSeen = static_cast< JsonObject* >(pValLastSeen);
	lastSeen->GetValue(pKeyTime, pValTime);
	JsonNumber *time = static_cast< JsonNumber* >(pValTime);

	String *pFirstName = new String(firstName->GetPointer());
	String *pLastName = new String(lastName->GetPointer());
	String *pMiniPhoto = new String(miniPhoto->GetPointer());
	String *pPhoto = new String(photo->GetPointer());

	user->SetFirstName(pFirstName);
	user->SetLastName(pLastName);
	user->SetMiniPhoto(pMiniPhoto);
	user->SetPhoto(pPhoto);
	user->SetUid(uid->ToInt());
	user->SetIsOnline(isOnline->ToInt());
	user->SetLastSeen(time->ToLong());
	user->__isFriend = isFriend->ToInt();

	delete pKeyFirstName;
	delete pKeyLastName;
	delete pKeyId;
	delete pKeyMiniPhoto;
	delete pKeyPhoto;
	delete pKeyOnline;
	delete pKeyLastSeen;
	delete pKeyTime;
	delete pKeyIsFriend;

	return user;
}

String*
MUser::TableDescription() {

	String *sql = new String();

	sql->Append(L"CREATE TABLE IF NOT EXISTS users (_id INTEGER PRIMARY KEY, uid INTEGER UNIQUE, last_name TEXT, first_name TEXT, photo TEXT, mini_photo TEXT, is_online INTEGER, last_seen INTEGER, is_friend  INTEGER, is_contact INTEGER, is_pending INTEGER)");

	return sql;
}

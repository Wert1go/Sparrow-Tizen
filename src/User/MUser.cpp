	/*
 * User.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#include "MUser.h"
#include <FSocial.h>
#include "Helper.h"

using namespace Tizen::Social;
using namespace Tizen::Base;
using namespace Tizen::Web::Json;

MUser::MUser() {
	__isFriend = 0;
	__isContact = 0;
	__isPending = 0;

	__lastSeen = 0;
	__chat = 0;

	__firstName = null;
	__lastName = null;
	__pBigPhoto = null;

	__pContactName = null;
	__pContactPhone = null;
	__pContactPhoto = null;

	__phoneNumber = null;
	__photo = null;
	__miniPhoto = null;
}

MUser::~MUser() {
	SAFE_DELETE(__firstName);
	SAFE_DELETE(__lastName);
	SAFE_DELETE(__pBigPhoto);

	SAFE_DELETE(__pContactName);
	SAFE_DELETE(__pContactPhone);
	SAFE_DELETE(__pContactPhoto);

	SAFE_DELETE(__phoneNumber);
	SAFE_DELETE(__photo);
	SAFE_DELETE(__miniPhoto);
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
	JsonString* pKeyBigPhoto = new JsonString(L"photo_200");
	JsonString* pKeyOnline = new JsonString(L"online");
	JsonString* pKeyLastSeen = new JsonString(L"last_seen");
	JsonString* pKeyTime = new JsonString(L"time");
	JsonString* pKeyIsFriend = new JsonString(L"is_friend");

	JsonString* pKeyPhone = new JsonString(L"phone");

	IJsonValue* pValFirstName = null;
	IJsonValue* pValLastName = null;
	IJsonValue* pValId = null;
	IJsonValue* pValMiniPhoto = null;
	IJsonValue* pValBigPhoto = null;
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
	pUserObject.GetValue(pKeyBigPhoto, pValBigPhoto);

	JsonString *firstName = static_cast< JsonString* >(pValFirstName);
	JsonString *lastName = static_cast< JsonString* >(pValLastName);
	JsonNumber *uid = static_cast< JsonNumber* >(pValId);
	JsonString *miniPhoto = static_cast< JsonString* >(pValMiniPhoto);


	JsonString *bigPhoto = null;
	if (pValBigPhoto) {
		bigPhoto = static_cast< JsonString* >(pValBigPhoto);
	}

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

	if (!bigPhoto) {
		bigPhoto = photo;
	}

	String *pBigPhoto = new String(bigPhoto->GetPointer());

	user->SetFirstName(pFirstName);
	user->SetLastName(pLastName);
	user->SetMiniPhoto(pMiniPhoto);
	user->SetPhoto(pPhoto);
	user->__pBigPhoto = pBigPhoto;
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

	IJsonValue* pValPhone = null;
	pUserObject.GetValue(pKeyPhone, pValPhone);
	if (pValPhone) {
		JsonString *phone = static_cast< JsonString* >(pValPhone);
		if (phone) {
			user->__pContactPhone = new String(phone->GetPointer());
		}
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
	delete pKeyBigPhoto;
	delete pKeyPhone;

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
	JsonNumber *isFriend = null;
	JsonObject *lastSeen = null;
	JsonNumber *time = null;

	if (isFriend) {
		isFriend = static_cast< JsonNumber* >(pValIsFriend);
	}

	if (lastSeen) {
		lastSeen = static_cast< JsonObject* >(pValLastSeen);
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
	user->__pBigPhoto = pPhoto;
	user->SetUid(uid->ToInt());
	user->SetIsOnline(isOnline->ToInt());

	if (time) {
		user->SetLastSeen(time->ToLong());
	}
	if (isFriend) {
		user->__isFriend = isFriend->ToInt();
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

MUser *
MUser::CreateFromPerson(Tizen::Social::Person *pPerson) {
	MUser *pUser = new MUser();

	String personName = pPerson->GetDisplayName();
	PhoneNumber phone = pPerson->GetPrimaryPhoneNumber();
	String phoneString = phone.GetPhoneNumber();

	AppLog("%S :: %S", personName.GetPointer(), phoneString.GetPointer());

	pUser->SetFirstName(new String(L""));
	pUser->SetLastName(new String(L""));
	pUser->SetPhoto(new String(L""));
	pUser->SetMiniPhoto(new String(L""));
	pUser->__isContact = 1;
	pUser->__pContactName = new String(personName.GetPointer());
	pUser->__pContactPhone = new String(phoneString.GetPointer());

	return pUser;
}

String*
MUser::TableDescription() {

	String *sql = new String();

	sql->Append(L"CREATE TABLE IF NOT EXISTS users ("
			"_id INTEGER PRIMARY KEY,"
			" uid INTEGER UNIQUE,"
			" last_name TEXT,"
			" first_name TEXT,"
			" photo TEXT,"
			" mini_photo TEXT,"
			" is_online INTEGER,"
			" last_seen INTEGER,"
			" is_friend  INTEGER,"
			" is_contact INTEGER,"
			" is_pending INTEGER,"
			" big_photo TEXT, "

			" contact_name TEXT, "
			" contact_phone TEXT, "
			" contact_photo TEXT "
			")");

	return sql;
}

String*
MUser::TableContactsDescription() {

	String *sql = new String();

	sql->Append(L"CREATE TABLE IF NOT EXISTS contacts ("
			"_id INTEGER PRIMARY KEY,"
			" uid INTEGER,"
			" last_name TEXT,"
			" first_name TEXT,"
			" photo TEXT,"
			" mini_photo TEXT,"
			" is_online INTEGER,"
			" last_seen INTEGER,"
			" is_friend  INTEGER,"
			" is_contact INTEGER,"
			" is_pending INTEGER,"
			" big_photo TEXT, "

			" contact_name TEXT, "
			" contact_phone TEXT, "
			" contact_photo TEXT "
			")");

	return sql;
}

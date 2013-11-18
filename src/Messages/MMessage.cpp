/*
 * MMessage.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "MMessage.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

MMessage::MMessage() {
	// TODO Auto-generated constructor stub

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
			" text TEXT"
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

	IJsonValue* pValMessageId = null;
	IJsonValue* pValFromId = null;
	IJsonValue* pValUserId = null;
	IJsonValue* pValDate = null;
	IJsonValue* pValOut = null;
	IJsonValue* pValReadState = null;
	IJsonValue* pValText = null;

	pObject.GetValue(pKeyId, pValMessageId);
	pObject.GetValue(pKeyFromId, pValFromId);
	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyOut, pValOut);
	pObject.GetValue(pKeyReadState, pValReadState);
	pObject.GetValue(pKeyText, pValText);

	JsonNumber *mid = static_cast< JsonNumber* >(pValMessageId);
	JsonNumber *fromId = static_cast< JsonNumber* >(pValFromId);
	JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
	JsonNumber *date = static_cast< JsonNumber* >(pValDate);
	JsonNumber *out = static_cast< JsonNumber* >(pValOut);
	JsonNumber *readState = static_cast< JsonNumber* >(pValReadState);
	JsonString *text = static_cast< JsonString* >(pValText);

	String *pText = new String(text->GetPointer());

	message->SetMid(mid->ToInt());
	message->SetFromUid(fromId->ToInt());
	message->SetUid(uid->ToInt());
	message->SetDate(date->ToLong());
	message->SetOut(out->ToInt());
	message->SetReadState(readState->ToInt());
	message->SetText(pText);

	delete pKeyId;
	delete pKeyUserId;
	delete pKeyDate;
	delete pKeyOut;
	delete pKeyReadState;
	delete pKeyFromId;
	delete pKeyText;

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

	IJsonValue* pValMessageId = null;
	IJsonValue* pValUserId = null;
	IJsonValue* pValDate = null;
	IJsonValue* pValOut = null;
	IJsonValue* pValReadState = null;
	IJsonValue* pValText = null;

	pObject.GetValue(pKeyId, pValMessageId);
	pObject.GetValue(pKeyUserId, pValUserId);
	pObject.GetValue(pKeyDate, pValDate);
	pObject.GetValue(pKeyOut, pValOut);
	pObject.GetValue(pKeyReadState, pValReadState);
	pObject.GetValue(pKeyText, pValText);

	JsonNumber *mid = static_cast< JsonNumber* >(pValMessageId);
	JsonNumber *uid = static_cast< JsonNumber* >(pValUserId);
	JsonNumber *date = static_cast< JsonNumber* >(pValDate);
	JsonNumber *out = static_cast< JsonNumber* >(pValOut);
	JsonNumber *readState = static_cast< JsonNumber* >(pValReadState);
	JsonString *text = static_cast< JsonString* >(pValText);

	String *pText = new String(text->GetPointer());

	message->SetMid(mid->ToInt());
	message->SetUid(uid->ToInt());
	message->SetDate(date->ToLong());
	message->SetOut(out->ToInt());
	message->SetReadState(readState->ToInt());
	message->SetText(pText);

	delete pKeyId;
	delete pKeyUserId;
	delete pKeyDate;
	delete pKeyOut;
	delete pKeyReadState;
	delete pKeyText;

	return message;
}

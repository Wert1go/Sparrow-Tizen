/*
 * LongPollObject.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollObject.h"

using namespace Tizen::Web::Json;

LongPollObject::LongPollObject() {
	// TODO Auto-generated constructor stub

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

LongPollObject*
LongPollObject::CreateFromJsonN(const Tizen::Web::Json::JsonArray &jsonArray) {

	LongPollObject *resultObject = new LongPollObject();
	IJsonValue* pValType;
	IJsonValue* pValUserId;

	JsonNumber *pUserId;

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
	case LP_FLAG_RESET:

			break;
	case LP_MESSAGE_ADD:

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

			break;
	case LP_USER_PRINT_MSG_CHAT:

			break;
	}

	return resultObject;
}

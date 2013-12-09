/*
 * MAttachment.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#include "MAttachment.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

const wchar_t* PHOTO = L"photo";

MAttachment::MAttachment() {
	__pType = null;
	__pAccessKey = null;
	__pPhoto130 = null;
	__pPhoto604 = null;

	__pFilePath = null;
	__tempId = 0;
	ratio = 0;
	imageSize = FloatPoint(0,0);
	__id = 0;

}

MAttachment::~MAttachment() {
	if (__pType) {
		delete __pType;
		__pType = null;
	}

	if (__pAccessKey) {
		delete __pAccessKey;
		__pAccessKey = null;
	}

	if (__pPhoto130) {
		delete __pPhoto130;
		__pPhoto130 = null;
	}

	if (__pPhoto604) {
		delete __pPhoto604;
		__pPhoto604 = null;
	}
}

MAttachment*
MAttachment::CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject) {

	JsonString* pKeyType = new JsonString(L"type");
	IJsonValue *pValType = null;

	jsonObject.GetValue(pKeyType, pValType);

	if (pValType) {
		JsonString *typeString = static_cast<JsonString *>(pValType);

		if(typeString->Equals(PHOTO, false)) {
			MAttachment *photo = null;

			JsonString* pKeyPhoto = new JsonString(PHOTO);
			IJsonValue *pValPhoto = null;

			jsonObject.GetValue(pKeyPhoto, pValPhoto);

			JsonObject *pPhotoObject = static_cast<JsonObject *>(pValPhoto);

			photo = MAttachment::CreatePhotoFromJsonN(pPhotoObject);

			return photo;
		}
	}

	delete pKeyType;

	return null;
}

MAttachment *
MAttachment::CreatePhotoFromJsonN(JsonObject *pPhotoObject) {
	MAttachment *photo = new MAttachment();

	JsonString* pKeyId = new JsonString(L"id");
	IJsonValue *pValId = null;
	JsonString* pKeyOwnerId = new JsonString(L"owner_id");
	IJsonValue *pValOwnerId = null;
	JsonString* pKeyDate = new JsonString(L"date");
	IJsonValue *pValDate = null;
	JsonString* pKeyAccess = new JsonString(L"access_key");
	IJsonValue *pValAccess = null;
	JsonString* pKeyWidth = new JsonString(L"width");
	IJsonValue *pValWidth = null;
	JsonString* pKeyHeight = new JsonString(L"height");
	IJsonValue *pValHeight = null;
	JsonString* pKeyPhoto130 = new JsonString(L"photo_130");
	IJsonValue *pValPhoto130 = null;
	JsonString* pKeyPhoto604 = new JsonString(L"photo_604");
	IJsonValue *pValPhoto604 = null;

	pPhotoObject->GetValue(pKeyId, pValId);
	pPhotoObject->GetValue(pKeyOwnerId, pValOwnerId);
	pPhotoObject->GetValue(pKeyDate, pValDate);
	pPhotoObject->GetValue(pKeyAccess, pValAccess);
	pPhotoObject->GetValue(pKeyWidth, pValWidth);
	pPhotoObject->GetValue(pKeyHeight, pValHeight);
	pPhotoObject->GetValue(pKeyPhoto130, pValPhoto130);
	pPhotoObject->GetValue(pKeyPhoto604, pValPhoto604);

	JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
	JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);
	JsonNumber *pDate = static_cast<JsonNumber *>(pValDate);
	JsonString *pAccess = static_cast<JsonString *>(pValAccess);

	JsonNumber *pWidth = static_cast<JsonNumber *>(pValWidth);
	JsonNumber *pHeight = static_cast<JsonNumber *>(pValHeight);

	JsonString *pPhoto130 = static_cast<JsonString *>(pValPhoto130);
	JsonString *pPhoto604 = static_cast<JsonString *>(pValPhoto604);

	photo->__id = pKey->ToInt();
	photo->__ownerId = pOwner->ToInt();
	photo->__date = pDate->ToInt();

	photo->__pType = new String("photo");

	if (pAccess) {
		photo->__pAccessKey = new String(pAccess->GetPointer());
	}

	photo->__width = pWidth->ToInt();
	photo->__height = pHeight->ToInt();

	if (pPhoto130) {
		photo->__pPhoto130 = new String(pPhoto130->GetPointer());
	}
	if (pPhoto604) {
		photo->__pPhoto604 = new String(pPhoto604->GetPointer());
	}

	delete pKeyId;
	delete pKeyOwnerId;
	delete pKeyDate;
	delete pKeyAccess;
	delete pKeyWidth;
	delete pKeyHeight;
	delete pKeyPhoto130;
	delete pKeyPhoto604;

	return photo;
}


MAttachment*
MAttachment::CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &jsonObject) {
	JsonString* pKeyType = new JsonString(L"type");
		IJsonValue *pValType = null;

		jsonObject.GetValue(pKeyType, pValType);

		if (pValType) {
			JsonString *typeString = static_cast<JsonString *>(pValType);

			if(typeString->Equals(PHOTO, false)) {
				MAttachment *photo = new MAttachment();

				JsonString* pKeyPhoto = new JsonString(PHOTO);
				IJsonValue *pValPhoto = null;

				jsonObject.GetValue(pKeyPhoto, pValPhoto);

				JsonObject *pPhotoObject = static_cast<JsonObject *>(pValPhoto);

				JsonString* pKeyId = new JsonString(L"pid");
				IJsonValue *pValId = null;
				JsonString* pKeyOwnerId = new JsonString(L"owner_id");
				IJsonValue *pValOwnerId = null;
				JsonString* pKeySizes = new JsonString(L"sizes");
				IJsonValue *pValSizes = null;

				pPhotoObject->GetValue(pKeySizes, pValSizes);

				pPhotoObject->GetValue(pKeyId, pValId);
				pPhotoObject->GetValue(pKeyOwnerId, pValOwnerId);

				JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
				JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);

				photo->__id = pKey->ToInt();
				photo->__ownerId = pOwner->ToInt();
				photo->__pType = new String(typeString->GetPointer());

				if (pValSizes) {
					JsonArray *pSizes = static_cast<JsonArray *>(pValSizes);

					JsonString* pKeySrc = new JsonString(L"src");
					IJsonValue *pValSrc = null;
					JsonString* pKeyWidth = new JsonString(L"width");
					IJsonValue *pValWidth = null;
					JsonString* pKeyHeight = new JsonString(L"height");
					IJsonValue *pValHeight = null;

					for (int i = 0; i < pSizes->GetCount(); i++) {
						JsonObject *pSize;
						IJsonValue *pValSize = null;
						pSizes->GetAt(i, pValSize);
						pSize = static_cast<JsonObject *>(pValSize);

						pSize->GetValue(pKeySrc, pValSrc);
						pSize->GetValue(pKeyWidth, pValWidth);
						pSize->GetValue(pKeyHeight, pValHeight);

						JsonNumber *width = static_cast<JsonNumber *>(pValWidth);
						JsonNumber *height = static_cast<JsonNumber *>(pValHeight);
						JsonString *src = static_cast<JsonString *>(pValSrc);

						photo->__width = width->ToInt();
						photo->__height = height->ToInt();

						if (height->ToInt() == 130 || width->ToInt() == 130) {
							photo->__pPhoto130 = new String(src->GetPointer());
						} else if (height->ToInt() == 604 || width->ToInt() == 604) {
							photo->__pPhoto604 = new String(src->GetPointer());
						}
					}
				}

				delete pKeyId;
				delete pKeyOwnerId;

				return photo;
			}
		}

		delete pKeyType;

		return null;
}

String*
MAttachment::TableDescription() {
	String *sql = new String();

	sql->Append(L""
			"CREATE TABLE IF NOT EXISTS "
			"attachments ("
				"_id INTEGER PRIMARY KEY, "
				"identifier INTEGER UNIQUE, "
				"mid INTEGER, "
				"type TEXT, "
				"owner_id INTEGER, "
				"date INTEGER, "
				"access_key TEXT, "
				"width  INTEGER, "
				"height INTEGER, "
				"photo_130 TEXT, "
				"photo_604 TEXT "
			")");

	return sql;
}

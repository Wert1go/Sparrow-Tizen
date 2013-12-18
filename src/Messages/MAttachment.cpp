/*
 * MAttachment.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#include "MAttachment.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

#include "Helper.h"
#include "MUser.h"
#include "MGeo.h"

const wchar_t* PHOTO = L"photo";
const wchar_t* AUDIO = L"audio";
const wchar_t* VIDEO = L"video";
const wchar_t* DOC = L"doc";
const wchar_t* POINT = L"point";
const wchar_t* FWD = L"fwd";

MAttachment::MAttachment() {
	__id = 0;
	__album_id = 0;

	__duration = 0;
	__views = 0;
	__size = 0;

	//utils

	__tempId = 0;
	ratio = 0;
	cut = 0;
	__nesting = 0;
	imageSize = FloatPoint(0,0);

	__pType = null;
	__pAccessKey = null;
	__pPhoto130 = null;
	__pPhoto604 = null;

	__pVideoPhoto320 = null;
	__pTitle = null;

	__pDescription = null;
	__pArtist = null;
	__pUrl = null;

	__pExt = null;

	__pFilePath = null;

	__pUser = null;
	__text = null;
	__pAttachments = null;
	__pGeo = null;
	__pFwd = null;
	__text = null;

}

MAttachment::~MAttachment() {
	SAFE_DELETE(__pType);
	SAFE_DELETE(__pAccessKey);
//	SAFE_DELETE(__pPhoto130);
//	SAFE_DELETE(__pPhoto604);
//	SAFE_DELETE(__pVideoPhoto320);
	SAFE_DELETE(__pTitle);
	SAFE_DELETE(__pDescription);
	SAFE_DELETE(__pArtist);
	SAFE_DELETE(__pUrl);
	SAFE_DELETE(__pExt);

	SAFE_DELETE(__text);

	SAFE_DELETE(__pUser);

	if (__pFwd) {
		__pFwd->RemoveAll(true);
		delete __pFwd;
		__pFwd = null;
	}

	if (__pAttachments) {
		__pAttachments->RemoveAll(true);
		delete __pAttachments;
		__pAttachments = null;
	}
//
//	SAFE_DELETE(__pGeo);
}

MAttachment*
MAttachment::CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject) {

	JsonString* pKeyType = new JsonString(L"type");
	IJsonValue *pValType = null;

	jsonObject.GetValue(pKeyType, pValType);

	if (pValType) {
		JsonString *typeString = static_cast<JsonString *>(pValType);

		if (typeString->Equals(PHOTO, false)) {
			MAttachment *photo = null;

			JsonString* pKeyPhoto = new JsonString(PHOTO);
			IJsonValue *pValPhoto = null;

			jsonObject.GetValue(pKeyPhoto, pValPhoto);

			JsonObject *pPhotoObject = static_cast<JsonObject *>(pValPhoto);

			photo = MAttachment::CreatePhotoFromJsonN(pPhotoObject);

			delete pKeyPhoto;

			return photo;
		} else if (typeString->Equals(VIDEO, false)) {
			MAttachment *pAttach = null;

			JsonString* pKeyAttach = new JsonString(VIDEO);
			IJsonValue *pValAttach = null;

			jsonObject.GetValue(pKeyAttach, pValAttach);

			JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);

			pAttach = MAttachment::CreateVideoFromJsonN(pAttachObject);

			delete pKeyAttach;

			return pAttach;
		} else if (typeString->Equals(AUDIO, false)) {
			MAttachment *pAttach = null;

			JsonString* pKeyAttach = new JsonString(AUDIO);
			IJsonValue *pValAttach = null;

			jsonObject.GetValue(pKeyAttach, pValAttach);

			if (pValAttach) {
				JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);
				pAttach = MAttachment::CreateAudioFromJsonN(pAttachObject);
			}

			delete pKeyAttach;

			return pAttach;
		} else if (typeString->Equals(DOC, false)) {
			MAttachment *pAttach = null;

			JsonString* pKeyAttach = new JsonString(DOC);
			IJsonValue *pValAttach = null;

			jsonObject.GetValue(pKeyAttach, pValAttach);

			if (pValAttach) {
				JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);
				pAttach = MAttachment::CreateDocFromJsonN(pAttachObject);
			}

			delete pKeyAttach;

			return pAttach;
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
	JsonString* pKeyPId = new JsonString(L"pid");
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
	JsonString* pKeySizes = new JsonString(L"sizes");
	IJsonValue *pValSizes = null;

	JsonString* pKeyAlbumId = new JsonString(L"album_id");
	IJsonValue *pValAlbumId = null;

	pPhotoObject->GetValue(pKeyId, pValId);
	if (!pValId) {
		pPhotoObject->GetValue(pKeyPId, pValId);
	}

	pPhotoObject->GetValue(pKeyOwnerId, pValOwnerId);
	pPhotoObject->GetValue(pKeyDate, pValDate);
	pPhotoObject->GetValue(pKeyAccess, pValAccess);
	pPhotoObject->GetValue(pKeyWidth, pValWidth);
	pPhotoObject->GetValue(pKeyHeight, pValHeight);
	pPhotoObject->GetValue(pKeyPhoto130, pValPhoto130);
	pPhotoObject->GetValue(pKeyPhoto604, pValPhoto604);
	pPhotoObject->GetValue(pKeyAlbumId, pValAlbumId);

	JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
	JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);
	photo->__id = pKey->ToInt();
	photo->__ownerId = pOwner->ToInt();
	photo->__pType = new String("photo");

	JsonNumber *pWidth = static_cast<JsonNumber *>(pValWidth);
	JsonNumber *pHeight = static_cast<JsonNumber *>(pValHeight);

	if (pWidth) {
		JsonNumber *pDate = static_cast<JsonNumber *>(pValDate);
		JsonString *pAccess = static_cast<JsonString *>(pValAccess);

		JsonString *pPhoto130 = static_cast<JsonString *>(pValPhoto130);
		JsonString *pPhoto604 = static_cast<JsonString *>(pValPhoto604);
		JsonNumber *pAlbumId = static_cast<JsonNumber *>(pValAlbumId);

		photo->__date = pDate->ToInt();

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

		photo->__album_id = pAlbumId->ToInt();
	} else {
		pPhotoObject->GetValue(pKeySizes, pValSizes);

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
				} else if (height->ToInt() <= 604 || width->ToInt() <= 604) {
					photo->__pPhoto604 = new String(src->GetPointer());
				}
			}
		}

	}

	delete pKeyId;
	delete pKeyPId;
	delete pKeySizes;
	delete pKeyOwnerId;
	delete pKeyDate;
	delete pKeyAccess;
	delete pKeyWidth;
	delete pKeyHeight;
	delete pKeyPhoto130;
	delete pKeyPhoto604;
	delete pKeyAlbumId;

	return photo;
}

MAttachment *
MAttachment::CreateVideoFromJsonN(JsonObject *pVideoObject) {
	MAttachment *pAttach = new MAttachment();

	JsonString* pKeyId = new JsonString(L"id");
	JsonString* pKeyVideoId = new JsonString(L"vid");
	IJsonValue *pValId = null;
	JsonString* pKeyOwnerId = new JsonString(L"owner_id");
	IJsonValue *pValOwnerId = null;
	JsonString* pKeyAccess = new JsonString(L"access_key");
	IJsonValue *pValAccess = null;

	JsonString* pKeyDate = new JsonString(L"date");
	IJsonValue *pValDate = null;
	JsonString* pKeyAlbumId = new JsonString(L"album_id");
	IJsonValue *pValAlbumId = null;

	JsonString* pKeyPhoto130 = new JsonString(L"photo_130");
	JsonString* pKeyImageSmall = new JsonString(L"image_small");
	IJsonValue *pValPhoto130 = null;
	JsonString* pKeyPhoto320 = new JsonString(L"photo_320");
	JsonString* pKeyImage = new JsonString(L"image");
	IJsonValue *pValPhoto320 = null;

	JsonString* pKeyTitle = new JsonString(L"title");
	IJsonValue *pValTitle = null;
	JsonString* pKeyDuration = new JsonString(L"duration");
	IJsonValue *pValDuration = null;
	JsonString* pKeyViews = new JsonString(L"views");
	IJsonValue *pValViews = null;
	JsonString* pKeyDescription = new JsonString(L"description");
	IJsonValue *pValDescription = null;

	pVideoObject->GetValue(pKeyId, pValId);
	if (!pValId) {
		pVideoObject->GetValue(pKeyVideoId, pValId);
	}
	pVideoObject->GetValue(pKeyOwnerId, pValOwnerId);
	pVideoObject->GetValue(pKeyAccess, pValAccess);

	pVideoObject->GetValue(pKeyDate, pValDate);
	pVideoObject->GetValue(pKeyAlbumId, pValAlbumId);

	pVideoObject->GetValue(pKeyPhoto130, pValPhoto130);
	if (!pValPhoto130) {
		pVideoObject->GetValue(pKeyImageSmall, pValPhoto130);
	}

	pVideoObject->GetValue(pKeyPhoto320, pValPhoto320);
	if (!pValPhoto320) {
		pVideoObject->GetValue(pKeyImage, pValPhoto320);
	}

	pVideoObject->GetValue(pKeyTitle, pValTitle);
	pVideoObject->GetValue(pKeyDuration, pValDuration);
	pVideoObject->GetValue(pKeyViews, pValViews);
	pVideoObject->GetValue(pKeyDescription, pValDescription);

	JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
	JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);
	JsonNumber *pDate = static_cast<JsonNumber *>(pValDate);
	JsonString *pAccess = static_cast<JsonString *>(pValAccess);

	JsonNumber *pAlbumId = static_cast<JsonNumber *>(pValAlbumId);
	JsonString *pPhoto130 = static_cast<JsonString *>(pValPhoto130);
	JsonString *pPhoto320 = static_cast<JsonString *>(pValPhoto320);

	JsonString *pTitle = static_cast<JsonString *>(pValTitle);
	JsonNumber *pDuration = static_cast<JsonNumber *>(pValDuration);
	JsonNumber *pViews = static_cast<JsonNumber *>(pValViews);
	JsonString *pDescription = static_cast<JsonString *>(pValDescription);

	pAttach->__pType = new String(VIDEO);
	pAttach->__id = pKey->ToInt();
	pAttach->__ownerId = pOwner->ToInt();
	if (pAccess) {
		pAttach->__pAccessKey = new String(pAccess->GetPointer());
	}

	pAttach->__date = pDate->ToInt();
	if (pAlbumId) {
		pAttach->__album_id = pAlbumId->ToInt();
	}

	if (pPhoto130) {
		pAttach->__pPhoto130 = new String(pPhoto130->GetPointer());
	}
	if (pPhoto320) {
		pAttach->__pVideoPhoto320 = new String(pPhoto320->GetPointer());
	}

	if (pTitle) {
		pAttach->__pTitle = new String(pTitle->GetPointer());
	}

	pAttach->__duration = pDuration->ToInt();
	pAttach->__views = pViews->ToInt();

	if (pDescription) {
		pAttach->__pDescription = new String(pDescription->GetPointer());
	}

	delete pKeyId;
	delete pKeyVideoId;
	delete pKeyOwnerId;
	delete pKeyDate;
	delete pKeyAccess;
	delete pKeyPhoto130;
	delete pKeyPhoto320;
	delete pKeyImageSmall;
	delete pKeyImage;
	delete pKeyAlbumId;

	delete pKeyTitle;
	delete pKeyDuration;
	delete pKeyViews;
	delete pKeyDescription;

	return pAttach;
}

MAttachment *
MAttachment::CreateAudioFromJsonN(Tizen::Web::Json::JsonObject *pAttachObject) {
	MAttachment *pAttach = new MAttachment();

	JsonString* pKeyId = new JsonString(L"id");
	JsonString* pKeyAudioId = new JsonString(L"aid");
	IJsonValue *pValId = null;
	JsonString* pKeyOwnerId = new JsonString(L"owner_id");
	IJsonValue *pValOwnerId = null;

	JsonString* pKeyTitle = new JsonString(L"title");
	IJsonValue *pValTitle = null;
	JsonString* pKeyDuration = new JsonString(L"duration");
	IJsonValue *pValDuration = null;

	JsonString* pKeyArtist = new JsonString(L"artist");
	IJsonValue *pValArtist = null;
	JsonString* pKeyUrl = new JsonString(L"url");
	IJsonValue *pValUrl = null;

	pAttachObject->GetValue(pKeyId, pValId);
	if (!pValId) {
		pAttachObject->GetValue(pKeyAudioId, pValId);
	}

	pAttachObject->GetValue(pKeyOwnerId, pValOwnerId);

	pAttachObject->GetValue(pKeyTitle, pValTitle);
	pAttachObject->GetValue(pKeyDuration, pValDuration);

	pAttachObject->GetValue(pKeyArtist, pValArtist);
	pAttachObject->GetValue(pKeyUrl, pValUrl);


	JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
	JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);

	JsonString *pTitle = static_cast<JsonString *>(pValTitle);
	JsonNumber *pDuration = static_cast<JsonNumber *>(pValDuration);

	JsonString *pArtist = static_cast<JsonString *>(pValArtist);
	JsonString *pUrl = static_cast<JsonString *>(pValUrl);

	pAttach->__pType = new String(AUDIO);
	pAttach->__id = pKey->ToInt();
	pAttach->__ownerId = pOwner->ToInt();

	if (pTitle) {
		pAttach->__pTitle = new String(pTitle->GetPointer());
	}

	if (pArtist) {
		pAttach->__pArtist = new String(pArtist->GetPointer());
	}

	if (pUrl) {
		pAttach->__pUrl = new String(pUrl->GetPointer());
	}

	pAttach->__duration = pDuration->ToInt();

	delete pKeyId;
	delete pKeyAudioId;
	delete pKeyOwnerId;

	delete pKeyTitle;
	delete pKeyDuration;

	delete pKeyArtist;
	delete pKeyUrl;

	return pAttach;
}

MAttachment *
MAttachment::CreateDocFromJsonN(Tizen::Web::Json::JsonObject *pAttachObject) {
	MAttachment *pAttach = new MAttachment();

	JsonString* pKeyId = new JsonString(L"id");
	JsonString* pKeyAudioId = new JsonString(L"did");
	IJsonValue *pValId = null;
	JsonString* pKeyOwnerId = new JsonString(L"owner_id");
	IJsonValue *pValOwnerId = null;

	JsonString* pKeyAccess = new JsonString(L"access_key");
	IJsonValue *pValAccess = null;
	JsonString* pKeyTitle = new JsonString(L"title");
	IJsonValue *pValTitle = null;

	JsonString* pKeySize = new JsonString(L"size");
	IJsonValue *pValSize = null;
	JsonString* pKeyExt = new JsonString(L"ext");
	IJsonValue *pValExt = null;

	JsonString* pKeyUrl = new JsonString(L"url");
	IJsonValue *pValUrl = null;

	pAttachObject->GetValue(pKeyId, pValId);
	if (!pValId) {
		pAttachObject->GetValue(pKeyAudioId, pValId);
	}

	pAttachObject->GetValue(pKeyAccess, pValAccess);
	pAttachObject->GetValue(pKeyOwnerId, pValOwnerId);

	pAttachObject->GetValue(pKeyTitle, pValTitle);
	pAttachObject->GetValue(pKeySize, pValSize);

	pAttachObject->GetValue(pKeyExt, pValExt);
	pAttachObject->GetValue(pKeyUrl, pValUrl);

	JsonNumber *pKey = static_cast<JsonNumber *>(pValId);
	JsonNumber *pOwner = static_cast<JsonNumber *>(pValOwnerId);
	JsonString *pAccess = static_cast<JsonString *>(pValAccess);

	JsonString *pTitle = static_cast<JsonString *>(pValTitle);
	JsonNumber *pSize = static_cast<JsonNumber *>(pValSize);

	JsonString *pExt = static_cast<JsonString *>(pValExt);
	JsonString *pUrl = static_cast<JsonString *>(pValUrl);

	pAttach->__pType = new String(DOC);
	pAttach->__id = pKey->ToInt();
	pAttach->__ownerId = pOwner->ToInt();

	if (pTitle) {
		pAttach->__pTitle = new String(pTitle->GetPointer());
	}

	if (pAccess) {
		pAttach->__pAccessKey = new String(pAccess->GetPointer());
	}

	if (pExt) {
		pAttach->__pExt = new String(pExt->GetPointer());
	}

	if (pUrl) {
		pAttach->__pUrl = new String(pUrl->GetPointer());
	}

	pAttach->__size = pSize->ToInt();

	delete pKeyId;
	delete pKeyAudioId;

	delete pKeyOwnerId;
	delete pKeyAccess;

	delete pKeyTitle;
	delete pKeySize;

	delete pKeyExt;
	delete pKeyUrl;

	return pAttach;
}


/******************************* CREATE LP OBJECTS ***************************/

MAttachment*
MAttachment::CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &jsonObject) {
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
			} else if(typeString->Equals(VIDEO, false)) {
				MAttachment *pAttach = null;

				JsonString* pKeyAttach = new JsonString(VIDEO);
				IJsonValue *pValAttach = null;

				jsonObject.GetValue(pKeyAttach, pValAttach);

				if (pValAttach) {
					JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);

					pAttach = MAttachment::CreateVideoFromJsonN(pAttachObject);
				}

				delete pKeyAttach;

				return pAttach;
			} else if (typeString->Equals(AUDIO, false)) {
				MAttachment *pAttach = null;

				JsonString* pKeyAttach = new JsonString(AUDIO);
				IJsonValue *pValAttach = null;

				jsonObject.GetValue(pKeyAttach, pValAttach);

				if (pValAttach) {
					JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);
					pAttach = MAttachment::CreateAudioFromJsonN(pAttachObject);
				}

				delete pKeyAttach;

				return pAttach;
			} else if (typeString->Equals(DOC, false)) {
				MAttachment *pAttach = null;

				JsonString* pKeyAttach = new JsonString(DOC);
				IJsonValue *pValAttach = null;

				jsonObject.GetValue(pKeyAttach, pValAttach);

				if (pValAttach) {
					JsonObject *pAttachObject = static_cast<JsonObject *>(pValAttach);
					pAttach = MAttachment::CreateDocFromJsonN(pAttachObject);
				}

				delete pKeyAttach;

				return pAttach;
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
				"photo_604 TEXT, "

				"album_id INTEGER, "

				"photo_320 TEXT, "
				"title TEXT, "
				"duration INTEGER, "
				"views INTEGER, "
				"description TEXT, "

				"artist TEXT, "
				"url TEXT, "
				"size TEXT, "
				"ext TEXT "
			")");

	return sql;
}

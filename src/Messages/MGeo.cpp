/*
 * MGeo.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: developer
 */

#include "MGeo.h"
#include "Helper.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Web::Json;

MGeo::MGeo() {
	__id = 0;
	__mid = 0;

	__pUrl = null;

	__pType = null;
	__pCoordinates = null;

	__pPlaceTitle = null;
	__pPlaceCountry = null;
	__pPlaceCity = null;

	/**************************************/
	__text = null;
	__pAttachments = null;
	__pFwd = null;
	__pGeo = null;
	__pUser = null;


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
	__pAttachments = null;
	__pGeo = null;
	__pFwd = null;
	__text = null;
}

MGeo::~MGeo() {
	SAFE_DELETE(__pType);
	SAFE_DELETE(__pCoordinates);
	SAFE_DELETE(__pPlaceTitle);
	SAFE_DELETE(__pPlaceCountry);
	SAFE_DELETE(__pPlaceCity);
	SAFE_DELETE(__pUrl);
}

MGeo*
MGeo::CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject) {
	MGeo *pGeo = new MGeo();

	JsonString* pKeyType = new JsonString(L"type");
	JsonString* pKeyCoordinates = new JsonString(L"coordinates");

	JsonString* pKeyPlace = new JsonString(L"place");

	JsonString* pKeyTitle = new JsonString(L"title");
	JsonString* pKeyCountry = new JsonString(L"country");
	JsonString* pKeyCity = new JsonString(L"city");

	IJsonValue* pValType = null;
	IJsonValue* pValCoordinates = null;

	IJsonValue* pValPlace = null;

	IJsonValue* pValTitle = null;
	IJsonValue* pValCountry = null;
	IJsonValue* pValCity = null;

	jsonObject.GetValue(pKeyType, pValType);
	jsonObject.GetValue(pKeyCoordinates, pValCoordinates);

	jsonObject.GetValue(pKeyPlace, pValPlace);

	JsonString *pTypeVal = static_cast<JsonString *>(pValType);
	JsonString *pCoordinatesVal = static_cast<JsonString *>(pValCoordinates);

	String *pType = new String(pTypeVal->GetPointer());
	String *pCoodinates = new String(pCoordinatesVal->GetPointer());

	JsonObject *pPlaceObject = static_cast<JsonObject *>(pValPlace);

	if (pPlaceObject) {
		pPlaceObject->GetValue(pKeyTitle, pValTitle);
		pPlaceObject->GetValue(pKeyCountry, pValCountry);
		pPlaceObject->GetValue(pKeyCity, pValCity);

		JsonString *pTitleVal = static_cast<JsonString *>(pValTitle);
		JsonString *pCountryVal = static_cast<JsonString *>(pValCountry);
		JsonString *pCityVal = static_cast<JsonString *>(pValCity);

		if (pTitleVal) {
			pGeo->SetPlaceTitle(new String(pTitleVal->GetPointer()));
			pGeo->__pPlaceCity = new String(pCityVal->GetPointer());
			pGeo->__pPlaceCountry = new String(pCountryVal->GetPointer());
		}
	}

	pGeo->__pType = pType;
	pGeo->__pCoordinates = pCoodinates;

	delete pKeyType;
	delete pKeyCoordinates;

	delete pKeyPlace;

	delete pKeyTitle;
	delete pKeyCountry;
	delete pKeyCity;

	return pGeo;
}

String *
MGeo::GetImageUrl() {
	if (!this->__pUrl && this->__pCoordinates) {
		String *pUrl = new String(L"");
		pUrl->Append(L"http://maps.googleapis.com/maps/api/staticmap?markers=");
		String encodedValue;
		UrlEncoder::Encode(__pCoordinates->GetPointer(), L"UTF-8", encodedValue);
		pUrl->Append(encodedValue.GetPointer());
		pUrl->Append(L"&zoom=11&size=320x240&sensor=false&language=ru");//TODO язык в зависимости от локали
		__pUrl = pUrl;
	}

	return this->__pUrl;
}

//*********************** *****************//

String*
MGeo::TableDescription() {
	String *sql = new String();

	sql->Append(L""
			"CREATE TABLE IF NOT EXISTS "
			"geo ("
				"_id INTEGER PRIMARY KEY, "
				"mid INTEGER UNIQUE, "
				"type TEXT, "
				"coordinates TEXT, "
				"place_title TEXT, "
				"place_country TEXT, "
				"place_city  TEXT "

			")");

	return sql;
}

void
MGeo::SetPlaceTitle(String *pPlaceTitle) {
	__pPlaceTitle = pPlaceTitle;
	__pTitle = pPlaceTitle;
}

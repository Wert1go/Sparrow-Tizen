/*
 * MGeo.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: developer
 */

#include "MGeo.h"

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
}

MGeo::~MGeo() {
	if (__pType) {
		delete __pType;
		__pType = null;
	}

	if (__pCoordinates) {
		delete __pCoordinates;
		__pType = null;
	}

	if (__pPlaceTitle) {
		delete __pPlaceTitle;
		__pPlaceTitle = null;
	}

	if (__pPlaceCountry) {
		delete __pPlaceCountry;
		__pPlaceCountry = null;
	}

	if (__pPlaceCity) {
		delete __pPlaceCity;
		__pPlaceCity = null;
	}
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

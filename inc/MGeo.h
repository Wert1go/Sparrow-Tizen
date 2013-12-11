/*
 * MGeo.h
 *
 *  Created on: Dec 11, 2013
 *      Author: developer
 */

#ifndef MGEO_H_
#define MGEO_H_

#include "MAttachment.h"

class MGeo: public MAttachment {
public:
	MGeo();
	virtual ~MGeo();

	int __id;
	int __mid;

//	String *__pType;

	String *__pCoordinates;

	String *__pPlaceTitle;
	String *__pPlaceCountry;
	String *__pPlaceCity;

	static MGeo* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);
	static String* TableDescription();

	void SetPlaceTitle(String *pPlaceTitle);

	//image
public:
	String *GetImageUrl();

private:
	String *__pUrl;

};

#endif /* MGEO_H_ */

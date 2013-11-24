/*
 * MAttachment.h
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#ifndef MATTACHMENT_H_
#define MATTACHMENT_H_

#include "Bean.h"

extern const wchar_t* PHOTO;

using namespace Tizen::Graphics;

class MAttachment
: public Bean {
public:
	MAttachment();
	virtual ~MAttachment();

	String *__pType;
	int __id;
	int __ownerId;
	int __date;
	String *__pAccessKey;
	String *__pPhoto130;
	String *__pPhoto604;
	int __width;
	int __height;

	float ratio;
	FloatPoint imageSize;

	int __mid;

	static MAttachment* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);
	static MAttachment* CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &jsonObject);
	static String* TableDescription();


	int album_id;
};

#endif /* MATTACHMENT_H_ */

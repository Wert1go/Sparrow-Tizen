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
extern const wchar_t* AUDIO;
extern const wchar_t* VIDEO;
extern const wchar_t* DOC;
extern const wchar_t* POINT;
extern const wchar_t* FWD;

using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class MGeo;
class MUser;

class MUser;

class MAttachment
: public Bean {
public:
	MAttachment();
	virtual ~MAttachment();

public:
	//util
	int __tempId;
	String *__pFilePath;
	float ratio;
	FloatPoint imageSize;
	int __mid;

	//common
	String *__pType;
	int __id;
	int __ownerId;

	//image, video
	int __date;
	int __album_id;

	//image, video, doc
	String *__pAccessKey;

	//image, video
	String *__pPhoto130;

	//video
	String *__pVideoPhoto320;

	//image
	String *__pPhoto604;
	int __width;
	int __height;

	//video, audio, doc
	String *__pTitle;

	//video, audio
	int __duration;

	//video
	int __views;
	String *__pDescription;

	//audio
	String *__pArtist;

	//audio, doc
	String *__pUrl;

	//doc
	int __size;
	String *__pExt;

	int __nesting;

	String *__text;

public:
	static MAttachment* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);

	static MAttachment * CreatePhotoFromJsonN(Tizen::Web::Json::JsonObject *pPhotoObject);
	static MAttachment * CreateVideoFromJsonN(Tizen::Web::Json::JsonObject *pVideoObject);

	static MAttachment * CreateAudioFromJsonN(Tizen::Web::Json::JsonObject *pAttachObject);
	static MAttachment * CreateDocFromJsonN(Tizen::Web::Json::JsonObject *pAttachObject);

	static MAttachment* CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &jsonObject);
	static String* TableDescription();

	Point __absolutePosition;

	MUser *__pUser;
	MGeo *__pGeo;

	LinkedList *__pAttachments;
	LinkedList *__pFwd;

	int cut;

};

#endif /* MATTACHMENT_H_ */

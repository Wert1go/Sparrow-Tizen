/*
 * MMessage.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef MMESSAGE_H_
#define MMESSAGE_H_

#include <FBase.h>
#include "Bean.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class MGeo;

class MMessage
 : public Bean
{
public:
	MMessage();
	virtual ~MMessage();

	int __id;
private:
	//message info part

	int __mid;
	int __uid;
	int __fromUid;
	int __out;
	int __readState;
	String *__text;

	int __delivered;

	int __chatId;

public:
	long __date;
	String *uids;
	String *__title;
	int userCount;
	int adminId;

	MGeo *__pGeo;

	LinkedList *__pAttachments;

public:
	static String* TableDescription();
	static String* RelationTableDescription();

	int GetMid();
	int GetUid();
	int GetFromUid();
	long GetDate();
	int GetOut();
	int GetDelivered();
	int GetReadState();
	String *GetText();
	String *GetUids();
	int GetChatId();

	void SetMid(int id);
	void SetUid(int id);
	void SetFromUid(int id);
	void SetDate(long date);
	void SetOut(int out);
	void SetDelivered(int delivered);
	void SetReadState(int state);
	void SetText(String *text);
	void SetChatId(int chatId);

	static MMessage * CreateFromJsonN(const Tizen::Web::Json::JsonObject &pObject);
	static MMessage * CreateFromJsonLPN(const Tizen::Web::Json::JsonObject &pObject);


};

#endif /* MMESSAGE_H_ */

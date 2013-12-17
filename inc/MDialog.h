/*
 * MDialog.h
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#ifndef MDIALOG_H_
#define MDIALOG_H_

#include <FBase.h>
#include "Bean.h"

class MUser;

extern const int isChatValue;

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class MDialog
 : public Bean {
public:
	MDialog();
	virtual ~MDialog();

	static String* TableDescription();

private:
	//user info part
	String *__firstName;
	String *__lastName;
	String *__photo;
	String *__miniPhoto;

	int __uid;
	int __isOnline;

	//message info part
	int __identifier;
	long __date;
	int __out;
	int __readState;
	String *__title;
	String *__text;

	int __chatId;
	String *chatUids;

	LinkedList *__pUsers;
	MUser *__pUser;



public:
	String *__pType;

	int __attachmentCount;
	int __fwdCount;

	//Getters
	String * GetFirstName();
	String * GetLastName();
	String *GetPhoto();
	String *GetMiniPhoto();
	int GetUid();
	int GetIsOnline();

	int GetIdentifier();
	long GetDate();
	int GetOut();
	int GetReadState();
	String *GetTitle();
	String *GetText();

	int GetChatId();
	String *GetChatUids();
	ArrayList * GetChatUidsArray();

	//Setters
	LinkedList *GetUsers();
	void SetUsers(LinkedList *list);
	void SetFirstName(String *firstName);
	void SetLastName(String *lastName);
	void SetPhoto(String *photo);
	void SetMiniPhoto(String *miniPhoto);
	void SetUid(int uid);
	void SetIsOnline(int isOnline);

	void SetIdentifier(int id);
	void SetDate(long date);
	void SetOut(int out);
	void SetReadState(int state);
	void SetTitle(String *title);
	void SetText(String *text);

	void SetChatId(int id);
	void SetChatUids(String *uids);

	MUser * GetUser();

	static MDialog * CreateFromJsonN(
			const Tizen::Web::Json::JsonObject &pUserObject,
			const Tizen::Web::Json::JsonObject &pMessageObject);

	static MDialog * CreateSearchDialogFromJsonN(
				const Tizen::Web::Json::JsonObject &pObject);

	static MDialog * CreateFromUserN(MUser *pUser);

};

#endif /* MDIALOG_H_ */

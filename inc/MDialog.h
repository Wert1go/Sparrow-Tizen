/*
 * MDialog.h
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#ifndef MDIALOG_H_
#define MDIALOG_H_

#include <FBase.h>

using namespace Tizen::Base;

class MDialog {
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

public:
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

	//Setters

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

};

#endif /* MDIALOG_H_ */

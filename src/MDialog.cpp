/*
 * MDialog.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "MDialog.h"

MDialog::MDialog() {
	// TODO Auto-generated constructor stub

}

MDialog::~MDialog() {
	// TODO Auto-generated destructor stub
}

String*
MDialog::TableDescription() {
	String *sql = new String();
	sql->Append(L"CREATE TABLE IF NOT EXISTS dialogs (_id INTEGER PRIMARY KEY, identifier INTEGER UNIQUE, uid INTEGER, last_name TEXT, first_name TEXT, photo TEXT, mini_photo TEXT, is_online INTEGER, date INTEGER, out INTEGER, read_state INTEGER, title TEXT, text TEXT)");
	return sql;
}

/***************** GETTERS ******************/
String *
MDialog::GetFirstName() {
	return __firstName;
}

String *
MDialog::GetLastName() {
	return __lastName;
}

String *
MDialog::GetPhoto() {
	return __photo;
}

String *
MDialog::GetMiniPhoto() {
	return __miniPhoto;
}

int
MDialog::GetUid() {
	return __uid;
}

int
MDialog::GetIsOnline() {
	return __isOnline;
}

int
MDialog::GetIdentifier() {
	return __identifier;
}

long
MDialog::GetDate() {
	return __date;
}

int
MDialog::GetOut() {
	return __out;
}

int
MDialog::GetReadState() {
	return __readState;
}

String *
MDialog::GetTitle() {
	return __title;
}

String *
MDialog::GetText() {
	return __text;
}

/***************** SETTERS ******************/

void
MDialog::SetFirstName(String *firstName) {
	__firstName = firstName;
}

void
MDialog::SetLastName(String *lastName) {
	__lastName = lastName;
}

void
MDialog::SetPhoto(String *photo) {
	__photo = photo;
}

void
MDialog::SetMiniPhoto(String *miniPhoto) {
	__miniPhoto = miniPhoto;
}

void
MDialog::SetUid(int uid) {
	__uid = uid;
}

void
MDialog::SetIsOnline(int isOnline) {
	__isOnline = isOnline;
}

void
MDialog::SetIdentifier(int id) {
	__identifier = id;
}

void
MDialog::SetDate(long date) {
	__date = date;
}

void
MDialog::SetOut(int out) {
	__out = out;
}

void
MDialog::SetReadState(int state) {
	__readState = state;
}

void
MDialog::SetTitle(String *title) {
	__title = title;
}

void
MDialog::SetText(String *text) {
	__text = text;
}



/***************** SETTERS ******************/


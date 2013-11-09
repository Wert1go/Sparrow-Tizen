/*
 * User.h
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#ifndef USER_H_
#define USER_H_

#include <FBase.h>
#include "Bean.h"

using namespace Tizen::Base;

#define OWNER 	23
#define FRIEND	24
#define MEMBER  25
#define CONTACT 26



class MUser
 : Bean
{
public:
	MUser();
	virtual ~MUser();

	static MUser* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);
	static String* TableDescription();

private:
	String *__firstName;
	String *__lastName;
	String *__phoneNumber;
	String *__photo;
	String *__miniPhoto;
	int __uid;
	long __lastSeen;
	int __type;

	int __isOnline;

	int __sex;

public:
	String * GetFirstName();
	String * GetLastName();
	String *GetPhoto();
	String *GetMiniPhoto();
	int GetUid();
	long GetLastSeen();
	int GetIsOnline();
	int GetType();

	void SetFirstName(String *firstName);
	void SetLastName(String *lastName);
	void SetPhoto(String *photo);
	void SetMiniPhoto(String *miniPhoto);
	void SetUid(int uid);
	void SetLastSeen(long lastSeen);
	void SetIsOnline(int isOnline);
	void SetType(int type);

};

#endif /* USER_H_ */

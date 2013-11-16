/*
 * LongPollObject.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLOBJECT_H_
#define LONGPOLLOBJECT_H_

#include "Bean.h"

static const int LP_DELETE = 				0;
static const int LP_FLAG_REPLACE = 		1;
static const int LP_FLAG_ADD = 			2;
static const int LP_FLAG_RESET =			3;
static const int LP_MESSAGE_ADD =			4;
static const int LP_USER_ONLINE =			8;
static const int LP_USER_OFFLINE =		9;

static const int LP_CHAT_UPDATED =		51;

static const int LP_USER_PRINT_MSG =		61;
static const int LP_USER_PRINT_MSG_CHAT =	62;

static const int LP_VOIP =				70;

using namespace Tizen::Base;

class LongPollObject
 : public Bean
{
public:
	LongPollObject();
	virtual ~LongPollObject();

	void SetType(int type);
	int GetType();

	void SetUserId(int userId);
	int GetUserId();

	static LongPollObject* CreateFromJsonN(const Tizen::Web::Json::JsonArray &jsonArray);

private:
	int __type;
	int __userId;


};

#endif /* LONGPOLLOBJECT_H_ */

/*
 * Util.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <FBase.h>

using namespace Tizen::Base;

class Util
 : public Object
{
private:
	Util();
	virtual ~Util();

public:
	//НЕ работает с русскими символами
	static String* MD5N(String *string);
	static String* formatDateN(long date);
	static void LogE();

private:
	static String LeftPadZero(String s, int wantedLen);

};

#endif /* UTIL_H_ */

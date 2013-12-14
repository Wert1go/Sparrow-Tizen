/*
 * Util.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <FBase.h>
#include <FGraphics.h>

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class MMessage;

extern const int limitSize;

extern const int msgImageOffset;
extern const int imageSizeMedium;
extern const int imageSizeSmall;

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

	static Dimension CalculateDimensionForMessage(MMessage *message, bool fwd = false, int nesting = 0);

private:
	static String LeftPadZero(String s, int wantedLen);

};

#endif /* UTIL_H_ */

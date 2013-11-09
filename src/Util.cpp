/*
 * Util.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#include "Util.h"
#include <FBase.h>
#include <FSecurity.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Security::Crypto;

Util::Util() {
	// TODO Auto-generated constructor stub

}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

String *
Util::MD5N(String *string) {
	ByteBuffer* pInput = StringUtil::StringToUtf8N(string->GetPointer());
	pInput->SetLimit(pInput->GetCapacity() - 1);

	Md5Hash md5;
	ByteBuffer* pOutput = md5.GetHashN(*pInput);
	String sHash;
	{
		int byteCount = pOutput->GetLimit();

		for (int i = 0; i < byteCount; i++) {
			byte b;  pOutput -> GetByte(i, b);
			unsigned int ui = b;
			String sHex;
			sHex.Format(25, L"%02x", ui);
			sHash.Append(sHex);
		}
	}

	delete pInput;
	delete pOutput;

	sHash = LeftPadZero(sHash, 32);

	return new String(sHash.GetPointer());
}

String
Util::LeftPadZero(String s, int wantedLen) {
	int prependCount = (wantedLen - s.GetLength());
	if (prependCount <= 0) {
		return s; // nothing to do
	}

	String sPreffix = "0";
	for (int i=1; i < prependCount; i++) {
		sPreffix = "0" + sPreffix;
	}

	return sPreffix + s;
}


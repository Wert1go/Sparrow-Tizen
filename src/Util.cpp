/*
 * Util.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#include "Util.h"
#include <FBase.h>
#include <FSecurity.h>
#include <FLocales.h>
#include <FSystem.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Security::Crypto;

using namespace Tizen::Locales;
using namespace Tizen::System;

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

void
Util::LogE() {
	result r = GetLastResult();
	AppLogDebug(GetErrorMessage(r));
}

String*
Util::formatDateN(long date) {
	String *stringDate = new String();

	 DateTime today;
	 Locale locale(LANGUAGE_RUS, COUNTRY_RU);
	 SystemTime::GetCurrentTime(TIME_MODE_UTC, today);
	 AppLogDebug("%d", today.GetSecond());
	 	 AppLogDebug("%d", date);
	 double delta = (today.GetSecond() - date)/(24 * 60 * 60);

	 today.AddSeconds(-(today.GetSecond() - date));
	 LocaleManager localeManager;
	 localeManager.Construct();

	 TimeZone timeZone = localeManager.GetSystemTimeZone();
	 today = timeZone.UtcTimeToStandardTime(today);

	 String timeZoneId = timeZone.GetId();

	 AppLogDebug("timeZoneId: %S", timeZoneId.GetPointer());

	 if (delta < 1) {
		 AppLogDebug("111!!pre");
		 DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(locale, DATE_TIME_STYLE_DEFAULT);
		 String cutomizedPattern = L"HH:mm";
		 pTimeFormatter->ApplyPattern(cutomizedPattern);

		 pTimeFormatter->Format(today, *stringDate);
	 } else if (delta > 0 && delta < 2) {
		 AppLogDebug("222!!pre");
		 stringDate = new String (L"вчера");
	 } else {
		 AppLogDebug("33!!pre");
		 DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(locale, DATE_TIME_STYLE_DEFAULT);
		 String cutomizedPattern = L"dd.MM";
		 pTimeFormatter->ApplyPattern(cutomizedPattern);
		 pTimeFormatter->Format(today, *stringDate);
	 }
	 AppLogDebug("11!!pre");

	return stringDate;
}

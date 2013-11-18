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
#include <FGraphics.h>
#include <iostream>
#include <sys/time.h>
#include "MMessage.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Security::Crypto;

using namespace Tizen::Locales;
using namespace Tizen::System;

using namespace Tizen::Graphics;

const int limitSize = 400;//720 - 190 - 130

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

	 unsigned long int timestamp = time(NULL);
	 long int delta = (timestamp - date)/(24 * 60 * 60);

	 LocaleManager localeManager;
	 localeManager.Construct();

	 DateTime after;
	 after.AddSeconds(date);

	 TimeZone timeZone = localeManager.GetSystemTimeZone();
	 after = timeZone.UtcTimeToStandardTime(after);

	 String timeZoneId = timeZone.GetId();

	 if (delta < 1) {
		 DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(locale, DATE_TIME_STYLE_DEFAULT);
		 String cutomizedPattern = L"HH:mm";
		 pTimeFormatter->ApplyPattern(cutomizedPattern);
		 pTimeFormatter->Format(after, *stringDate);
	 } else if (delta > 0 && delta < 2) {
		 stringDate = new String (L"вчера");
	 } else {
		 DateTimeFormatter* pTimeFormatter = DateTimeFormatter::CreateTimeFormatterN(locale, DATE_TIME_STYLE_DEFAULT);
		 String cutomizedPattern = L"dd.MM";
		 pTimeFormatter->ApplyPattern(cutomizedPattern);
		 pTimeFormatter->Format(after, *stringDate);
	 }

	return stringDate;
}

Dimension
Util::CalculateDimensionForMessage(MMessage *message) {
	EnrichedText* pTimeLabel = null;
	TextElement* pTImeText = null;

	pTimeLabel = new EnrichedText();
	pTimeLabel->Construct(Dimension(limitSize, 480));

	pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
	pTimeLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

	pTImeText = new TextElement();

	String *text = message->GetText();

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	pTImeText->Construct(*text);
		pTImeText->SetTextColor(Color(109, 110, 117, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 36);
			pTImeText->SetFont(font);
		}

	pTimeLabel->Add(*pTImeText);

	Dimension resultSize;

	FloatDimension size;
	int actualLength;
	pTimeLabel->GetTextExtent(0, text->GetLength(), size, actualLength);

	if (size.width <= limitSize) {
		resultSize.width = size.width;
		resultSize.height = size.height;
	} else {
		Dimension normalSize = pTimeLabel->GetTextExtent();
		resultSize = normalSize;
	}

	return resultSize;
}

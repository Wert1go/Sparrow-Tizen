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
#include "MAttachment.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Security::Crypto;

using namespace Tizen::Locales;
using namespace Tizen::System;

using namespace Tizen::Graphics;

const int limitSize = 400;//720 - 190 - 130
const int msgImageOffset = 20;
const int imageSizeMedium = 480;
const int imageSizeSmall = 130;

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

	 long int timestamp = time(NULL);
	 long int delta = (long int)((timestamp - (long int)date)/(24 * 60 * 60));

//	 AppLog("formatDateN %ld || %ld :: %ld", delta, timestamp, date);

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


	String *text = message->GetText();
	Dimension resultSize;

	if (text->GetLength() != 0) {
		EnrichedText* pTimeLabel = null;
		TextElement* pTImeText = null;

		pTimeLabel = new EnrichedText();
		pTimeLabel->Construct(Dimension(limitSize, 480));

		pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pTimeLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

		pTImeText = new TextElement();

		pTImeText->Construct(*text);
			pTImeText->SetTextColor(Color(109, 110, 117, 255));
			{
				Font font;
				font.Construct(FONT_STYLE_BOLD, 36);
				pTImeText->SetFont(font);
			}

		pTimeLabel->Add(*pTImeText);



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

	} else {
		resultSize = Dimension(0, 0);
	}

	if (message->__pAttachments && message->__pAttachments->GetCount()) {
		for (int i = 0; i < message->__pAttachments->GetCount(); i++) {
			MAttachment *attachment = static_cast<MAttachment *>( message->__pAttachments->GetAt(i));

			attachment->ratio = (float)attachment->__width/attachment->__height;

			float imgWidth;
			float imgHeight;

			AppLog("attachment->ratio %f", attachment->ratio);

			if (attachment->__pPhoto604) {
				if (attachment->__width > attachment->__height) {
					imgWidth = imageSizeMedium;
					imgHeight = imageSizeMedium / attachment->ratio;
				} else {
					imgHeight = imageSizeMedium;
					imgWidth = imageSizeMedium * attachment->ratio;
				}
			} else {
				if (attachment->__width > attachment->__height) {
					imgWidth = imageSizeSmall;
					imgHeight = imageSizeSmall / attachment->ratio;
				} else {
					imgHeight = imageSizeSmall;
					imgWidth = imageSizeSmall * attachment->ratio;
				}
			}

			attachment->imageSize = FloatPoint(imgWidth, imgHeight);

			if (resultSize.width < imgWidth) {
				resultSize.width = imgWidth;
			}

			resultSize.height += imgHeight;
			if (i != message->__pAttachments->GetCount() - 1) {
				resultSize.height += msgImageOffset;
			}
		}
	}


	return resultSize;
}

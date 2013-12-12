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
#include "MGeo.h"

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
Util::CalculateDimensionForMessage(MMessage *message, bool fwd) {


	if(message->__pFwd) {
		AppLog("message:: %d", message->__pFwd->GetCount());
	}

	String *text = message->GetText();
	Dimension resultSize;

	if (fwd) {
		resultSize.width = limitSize;
		resultSize.height += 80;
	}

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
		Dimension normalSize = pTimeLabel->GetTextExtent();

		if (size.width <= limitSize && normalSize.height <= size.height) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		} else {
			resultSize = normalSize;
		}

	} else {
		resultSize = Dimension(0, 0);
	}

	if (message->__pAttachments && message->__pAttachments->GetCount()) {
		for (int i = 0; i < message->__pAttachments->GetCount(); i++) {
			MAttachment *attachment = static_cast<MAttachment *>( message->__pAttachments->GetAt(i));

			AppLog("attachment: %S", attachment->__pType->GetPointer());

			attachment->ratio = (float)attachment->__width/attachment->__height;

			float imgWidth;
			float imgHeight;

			if (attachment->__pType->Equals(PHOTO, false)) {

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

			} else if (attachment->__pType->Equals(VIDEO, false)) {
				if (attachment->__pVideoPhoto320) {
					imgWidth = 320;
					imgHeight = 240;
				} else {
					imgWidth = 130;
					imgHeight = 98;
				}
			} else if (attachment->__pType->Equals(AUDIO, false)) {
				imgWidth = 420;
				imgHeight = 80;
			} else if (attachment->__pType->Equals(DOC, false)) {
				imgWidth = 320;
				imgHeight = 80;
			}

			if (resultSize.width < imgWidth) {
				resultSize.width = imgWidth;
			}

			attachment->imageSize = FloatPoint(imgWidth, imgHeight);

			resultSize.height += imgHeight;
			if (i != message->__pAttachments->GetCount() - 1) {
				resultSize.height += msgImageOffset;
			}
		}
	}

	if (message->__pGeo) {
		float imgWidth = 320;
		float imgHeight = 240;

		if (resultSize.width < imgWidth) {
			resultSize.width = imgWidth;
		}

		message->__pGeo->imageSize = FloatPoint(imgWidth, imgHeight);
		resultSize.height += imgHeight + 20;
	}

	if (message->__pFwd) {
		for(int i = 0; i < message->__pFwd->GetCount(); i++) {
			MMessage *pFwdMessage = static_cast<MMessage *>(message->__pFwd->GetAt(i));

			if (pFwdMessage->__pType) {
				AppLog("+++++++=======================++++++++++++++++++++++++++++++++++++++++++++++++++++++");
			}

			Dimension fwdDimension = Util::CalculateDimensionForMessage(pFwdMessage, true);

			AppLog("%d :: %d", fwdDimension.width, fwdDimension.height);

			if (resultSize.width < fwdDimension.width) {
				resultSize.width = fwdDimension.width;
			}

			resultSize.width += 20; // под оступ!

			pFwdMessage->imageSize = FloatPoint(fwdDimension.width, fwdDimension.height);

			resultSize.height += fwdDimension.height + 20;
		}
	}

	return resultSize;
}

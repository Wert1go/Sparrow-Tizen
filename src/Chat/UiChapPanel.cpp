/*
 * UiChapPanel.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: developer
 */

#include <FApp.h>
#include <FMedia.h>
#include <FGraphics.h>

#include "UiChapPanel.h"
#include "Resources.h"
#include "ImageCache.h"
#include "MDialog.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

UiChapPanel::UiChapPanel() {
	Image image;
	image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_header.png";
	this->__pRounder = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
	__pOnlineIcon = Resources::getInstance().GetOnlineIndicator();
	__pDialog = null;
	__IsOinline = false;
	__ChatIcon = null;
}

UiChapPanel::~UiChapPanel() {
	ImageCache::getInstance().CancelLoadingForTarget(this);
}

bool
UiChapPanel::Initialize(void) {
	result r = Construct(Rectangle(0, 0, 10, 10));
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	return true;
}

void
UiChapPanel::OnImageLoadedN(Bitmap *result, Integer *code) {
	this->__ChatIcon = result;
	this->SendUserEvent(0, 0);
	Tizen::App::App::GetInstance()->SendUserEvent(0, 0);
}

void
UiChapPanel::OnErrorN(Error *error) {
	__ChatIcon = null;
}

void
UiChapPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	this->RequestRedraw();
}

void
UiChapPanel::SetIsOnline(bool online) {
	__IsOinline = online;
	this->RequestRedraw();
}

void
UiChapPanel::SetDialog(MDialog *dialog) {
	__pDialog = dialog;
	if (__pDialog) {
		__IsOinline = __pDialog->GetIsOnline() == 1;
		ImageCache::getInstance().LoadImageForTarget(__pDialog->GetPhoto(), this);
	}
	this->RequestRedraw();
}

result
UiChapPanel::OnDraw() {
	result r = E_SUCCESS;

	Canvas* pCanvas = GetCanvasN();

	int imgSize = 84;
	int offset = 15;

	if (pCanvas != null)
	{
		Rectangle rect = GetBounds();

		pCanvas->FillRectangle(Color(65, 97, 137, 255), rect);

		if (__pDialog) {
			bool isChat = this->__pDialog->GetUid() > 2000000000;
			AppLogDebug("lolololo");
			if (!isChat) {
				AppLogDebug("111lolololo");
				Bitmap *userAvatar = this->__ChatIcon;

				if (userAvatar) {
					pCanvas->DrawBitmap(Rectangle(offset, rect.height/2 - imgSize/2, imgSize, imgSize), *userAvatar);
				}

				Bitmap *pRounder = this->__pRounder;
				pCanvas->DrawBitmap(Rectangle(offset, rect.height/2 - imgSize/2, imgSize, imgSize), *pRounder);
			}

			EnrichedText* pUserName = null;
			TextElement* pUsetNameText = null;

			pUserName = new EnrichedText();
			r = pUserName->Construct(Dimension(300, 60));

			pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
			pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
			pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
			pUserName->SetTextAbbreviationEnabled(true);

			pUsetNameText = new TextElement();
			AppLogDebug("11lolololo");
			String *titleText;

			if (isChat) {
				titleText = this->__pDialog->GetTitle();
			} else {
				String *fullName = new String();
				fullName->Append(this->__pDialog->GetFirstName()->GetPointer());
				fullName->Append(L" ");
				fullName->Append(this->__pDialog->GetLastName()->GetPointer());
				titleText = fullName;
			}
			AppLogDebug("22lolololo");
			r = pUsetNameText->Construct(titleText->GetPointer());

			pUsetNameText->SetTextColor(Color(255, 255, 255, 255));
			{
				Font font;
				font.Construct(FONT_STYLE_BOLD, 36);
				pUsetNameText->SetFont(font);
			}
			pUserName->Add(*pUsetNameText);

			FloatDimension size;
			int actualLength;
			pUserName->GetTextExtent(0, titleText->GetLength(), size, actualLength);

			if (size.height > 68) {
				size.height = 41;
			}
			AppLogDebug("33lolololo");
			pUserName->SetSize(size);

			float textPosition =  10;

			float textOffset = offset;

			if (!isChat) {
				textOffset += (imgSize + offset);
			}

			pCanvas->DrawText(FloatPoint(textOffset, 10), *pUserName);

		/*********************** CHAT ICON ****************************/


		/*********************** USER ICON ****************************/



		/*********************** ONLINE STATUS ***********************/

			if (this->__IsOinline && !isChat) {
				float offlinePosition = (float)(textPosition + (float)size.height/2) - 44/2;
				pCanvas->DrawBitmap(Rectangle(offset + imgSize + offset + size.width, offlinePosition, 44, 44), *Resources::getInstance().GetOnlineIndicator());
			}

		/*********************** DIALOG TEXT ************************/
			AppLogDebug("444444");
			EnrichedText* pDescriptionLabel = null;
			TextElement* pDescriptionText = null;

			pDescriptionLabel = new EnrichedText();
			pDescriptionLabel->Construct(Dimension(300, 60));

			pDescriptionLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
			pDescriptionLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
			pDescriptionLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
			pDescriptionLabel->SetTextAbbreviationEnabled(true);

			pDescriptionText = new TextElement();

			String *dialogText = new String();
			AppLogDebug("5555555");
			if (isChat) {
				int count = this->__pDialog->GetUsers()->GetCount() + 1;
				String countString;
				countString.Format(10, L"%d", count);

				dialogText->Append(countString.GetPointer());
				dialogText->Append(L" участников");
			} else {
				if (this->__IsOinline) {
					dialogText = new String(L"Online");
				} else {
					dialogText = new String(L"Offline");
				}
			}AppLogDebug("66666");

			if (dialogText->GetLength() == 0) {
				dialogText = new String(L" ");
			}

			r = pDescriptionText->Construct(dialogText->GetPointer());
			GetErrorMessage(r);

			pDescriptionText->SetTextColor(Color(255, 255, 255, 255));
			{
				Font font;
				font.Construct(FONT_STYLE_BOLD, 32);
				pDescriptionText->SetFont(font);
			}
			pDescriptionLabel->Add(*pDescriptionText);

			FloatDimension textSize = pDescriptionLabel->GetTextExtentF();

			if (textSize.height > 68) {
				textSize.height = 41;
			}

			pDescriptionLabel->SetSize(textSize);
			AppLogDebug("7777777");
			//5 для красоты :/
			pCanvas->DrawText(FloatPoint(textOffset, 55), *pDescriptionLabel);
		}

	}

	delete pCanvas;

	return r;
}

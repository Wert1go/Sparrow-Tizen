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
#include "MUser.h"
#include "AppResourceId.h"

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
	__isEditMode = false;
	__ChatIcon = null;
	__pPrintingTimer = null;
	this->__isUserPrinting = false;
	__pPrintingMessage = null;


}

UiChapPanel::~UiChapPanel() {
	ImageCache::getInstance().CancelLoadingForTarget(this);
	if (__pPrintingMessage) {
		delete __pPrintingMessage;
	}

	if (this->__pPrintingTimer) {
		this->__pPrintingTimer->Cancel();
		delete this->__pPrintingTimer;
		this->__pPrintingTimer = null;
	}
}

bool
UiChapPanel::Initialize(void) {
	result r = Construct(Rectangle(0, 0, 10, 10));
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));

	__pEditButton = new (std::nothrow) Button();
	__pEditButton->Construct(Rectangle(10, 10, 100, 70));

	__pEditButton->SetActionId(10037);
	__pEditButton->SetTextColor(Color(255, 255, 255, 255));
	__pEditButton->SetHighlightedTextColor(Color(127, 127, 127, 127));
	__pEditButton->SetPressedTextColor(Color(127, 127, 127, 127));
	__pEditButton->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);


	Image buttonImage;
	buttonImage.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"empty_bg.png";
	Bitmap *pBackgroundBitmap = buttonImage.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	__pEditButton->SetNormalBackgroundBitmap(*pBackgroundBitmap);
	__pEditButton->SetHighlightedBackgroundBitmap(*pBackgroundBitmap);
	__pEditButton->SetPressedBackgroundBitmap(*pBackgroundBitmap);

	AddControl(__pEditButton);

	__pEditButton->SetShowState(false);

	__pGroupButton = new (std::nothrow) Button();
	__pGroupButton->Construct(Rectangle(10, 10, 100, 70));

	__pGroupButton->SetActionId(10034);
	__pGroupButton->SetTextColor(Color(255, 255, 255, 255));
	__pGroupButton->SetHighlightedTextColor(Color(127, 127, 127, 127));
	__pGroupButton->SetPressedTextColor(Color(127, 127, 127, 127));
	__pGroupButton->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);

	__pGroupButton->SetNormalBackgroundBitmap(*pBackgroundBitmap);
	__pGroupButton->SetHighlightedBackgroundBitmap(*pBackgroundBitmap);
	__pGroupButton->SetPressedBackgroundBitmap(*pBackgroundBitmap);

	AddControl(__pGroupButton);
	__pGroupButton->SetShowState(false);

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

	if (__pDialog && __pDialog->GetChatId() != 0) {
		__pGroupButton->SetShowState(true);

		__pGroupButton->SetTextSize(40);
		String count;
		if ( __pDialog->GetUsers()) {
			count.Format(10, L"%d", __pDialog->GetUsers()->GetCount() + 1);
		} else {
			count = L"0";
		}
		__pGroupButton->SetText(count);

		__pGroupButton->SetBounds(Rectangle(this->GetBounds().width - 150, 0, 120, this->GetBounds().height));
		this->__pGroupButton->SetNormalBitmap(Point(10, 100/2 - 72/2), *Resources::getInstance().LoadBitmapNamed(L"header_icon_group.png"));
	} else {
		__pEditButton->SetTextSize(28);
		this->__pEditButton->SetBounds(Rectangle(this->GetBounds().width - 150, 0, 140, this->GetBounds().height));
	}

	this->RequestRedraw();
}

result
UiChapPanel::OnDraw() {
	result r = E_SUCCESS;

	Canvas* pCanvas = GetCanvasN();

	int imgSize = 84;
	int offset = 15;
	int textWidth = this->GetBounds().width - 100 - imgSize;

	if (pCanvas != null)
	{
		Rectangle rect = GetBounds();

		pCanvas->FillRectangle(Color(65, 97, 137, 255), rect);

		if (__pDialog) {
			bool isChat = this->__pDialog->GetUid() > isChatValue;

			if (!isChat) {

				Bitmap *userAvatar = this->__ChatIcon;

				if (userAvatar) {
					pCanvas->DrawBitmap(Rectangle(
							offset,
							rect.height/2 - imgSize/2,
							imgSize,
							imgSize), *userAvatar);
				}

				Bitmap *pRounder = this->__pRounder;
				pCanvas->DrawBitmap(Rectangle(
						offset,
						rect.height/2 - imgSize/2,
						imgSize,
						imgSize), *pRounder);
			}

			EnrichedText* pUserName = null;
			TextElement* pUsetNameText = null;

			pUserName = new EnrichedText();
			r = pUserName->Construct(Dimension(textWidth, 60));

			pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
			pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
			pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
			pUserName->SetTextAbbreviationEnabled(true);

			pUsetNameText = new TextElement();

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

			pUserName->SetSize(size);

			float textPosition =  10;

			float textOffset = offset;

			if (!isChat) {
				textOffset += (imgSize + offset);
			}

			pCanvas->DrawText(FloatPoint(textOffset, 10), *pUserName);

			delete pUserName;

		/*********************** CHAT ICON ****************************/


		/*********************** USER ICON ****************************/


		/*********************** ONLINE STATUS ***********************/

			if (this->__IsOinline && !isChat) {
				float offlinePosition = (float)(textPosition + (float)size.height/2) - 44/2;
				pCanvas->DrawBitmap(Rectangle(offset + imgSize + offset + size.width, offlinePosition, 44, 44), *Resources::getInstance().GetOnlineIndicator());
			}

		/*********************** DIALOG TEXT ************************/

			EnrichedText* pDescriptionLabel = null;
			TextElement* pDescriptionText = null;

			pDescriptionLabel = new EnrichedText();
			pDescriptionLabel->Construct(Dimension(textWidth, 60));

			pDescriptionLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
			pDescriptionLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
			pDescriptionLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
			pDescriptionLabel->SetTextAbbreviationEnabled(true);

			pDescriptionText = new TextElement();

			String *dialogText = new String();

			if (isChat) {
				if (this->__isUserPrinting) {
					dialogText = new String(__pPrintingMessage->GetPointer());
				} else {
					int count = this->__pDialog->GetUsers()->GetCount() + 1;
					String countString;
					countString.Format(10, L"%d", count);

					dialogText->Append(countString.GetPointer());

					String memberString;
					Application::GetInstance()->GetAppResource()->GetString(IDS_MEMBER, memberString);

					String suffix1;
					String suffex2;
					String suffix;
					Application::GetInstance()->GetAppResource()->GetString(IDS_MEMBER_SUFFIX_1, suffix1);
					Application::GetInstance()->GetAppResource()->GetString(IDS_MEMBER_SUFFIX_2, suffex2);

					suffix = suffix1;
					if(count == 1 || (count%10 == 1 && count != 11)) {
						suffix = L"";
					} else if((count >= 2 && count <= 4) || (count > 21 && (count%10 >= 2 && count%10 <= 4))) {
						suffix = suffex2;
					}

					dialogText->Append(L" ");
					dialogText->Append(memberString);
					dialogText->Append(suffix);
				}
			} else {

				if (this->__isUserPrinting) {
					dialogText = new String(__pPrintingMessage->GetPointer());
				} else {
					if (this->__IsOinline) {
						String statusString;
						Application::GetInstance()->GetAppResource()->GetString(IDS_ONLINE, statusString);
						dialogText = new String(statusString);
					} else {
						String statusString;
						Application::GetInstance()->GetAppResource()->GetString(IDS_OFFLINE, statusString);
						dialogText = new String(statusString);
					}
				}
			}

			if (dialogText->GetLength() == 0) {
				dialogText = new String(L" ");
			}

			r = pDescriptionText->Construct(dialogText->GetPointer());
			GetErrorMessage(r);

			delete dialogText;

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

			//5 для красоты :/
			pCanvas->DrawText(FloatPoint(textOffset, 55), *pDescriptionLabel);

			delete pDescriptionText;
		}

	}


	if (this->__isEditMode) {
		String descrString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_CANCEL, descrString);
		__pEditButton->SetText(descrString);
	}

	delete pCanvas;

	return r;
}

void
UiChapPanel::SetUserPrinting(int userId) {
	this->__isUserPrinting = true;

	if (this->__pDialog->GetUid() > isChatValue) {

		if (this->__pDialog->GetUsers()) {
			if (__pPrintingMessage) {
				delete __pPrintingMessage;
				__pPrintingMessage = null;
			}

			for (int index = 0; index < this->__pDialog->GetUsers()->GetCount(); index++) {
				MUser *pUser = static_cast<MUser *>(this->__pDialog->GetUsers()->GetAt(index));

				if (userId == pUser->GetUid()) {

					String *fullName = new String();
					fullName->Append(pUser->GetFirstName()->GetPointer());
					fullName->Append(L" ");
					fullName->Append(pUser->GetLastName()->GetPointer());

					__pPrintingMessage = new String(fullName->GetPointer());
					String statusString;
					Application::GetInstance()->GetAppResource()->GetString(IDS_PRINTING, statusString);
					__pPrintingMessage->Append(L" ");
					__pPrintingMessage->Append(statusString);
					break;
				}
			}
		}

	}

	if (!__pPrintingMessage) {
		String statusString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_PRINTING, statusString);
		__pPrintingMessage = new String(statusString);
	}

	this->Invalidate(true);

	if (this->__pPrintingTimer) {
		this->__pPrintingTimer->Cancel();
		delete this->__pPrintingTimer;
		this->__pPrintingTimer = null;
	}

	this->__pPrintingTimer = new Timer();
	this->__pPrintingTimer->Construct(*this);
	this->__pPrintingTimer->Start(3000);
}

void
UiChapPanel::OnTimerExpired (Timer &timer) {
	this->__isUserPrinting = false;
	__IsOinline = true;

	this->Invalidate(true);

	delete this->__pPrintingTimer;
	this->__pPrintingTimer = null;
}

void
UiChapPanel::SetEditMode(bool mode) {
	this->__isEditMode = mode;

	if (mode) {
		if (this->__pGroupButton->GetShowState()) {
			this->__pGroupButton->SetShowState(false);
		}
		this->__pEditButton->SetShowState(true);
	} else {
		this->__pEditButton->SetShowState(false);
		if (this->__pDialog && this->__pDialog->GetChatId() != 0) {
			this->__pGroupButton->SetShowState(false);
		}
	}

	this->Invalidate(true);
}

bool
UiChapPanel::GetEditMode() {
	return this->__isEditMode;
}

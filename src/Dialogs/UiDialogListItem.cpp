/*
 * UiDialogListItem.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#include "UiDialogListItem.h"

#include <FApp.h>
#include "AppResourceId.h"

#include <FUi.h>
#include <FGraphics.h>
#include "MDialog.h"
#include "Util.h"

#include "Resources.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

static int baseListItemOffset = 150;

UiDialogListItem::UiDialogListItem() {
	__pTimeLabel = null;
	__pTimeText = null;

	__pUserName = null;
	__pUserNameText = null;

	__pDialogLabel = null;
	__pDialogText = null;
}

UiDialogListItem::~UiDialogListItem() {
	if (__pTimeText) {
		delete __pTimeText;
	}
	if (__pTimeLabel) {
		__pTimeLabel->RemoveAll(true);
		delete __pTimeLabel;
	}

	if (__pUserNameText) {
		delete __pUserNameText;
	}
	if (__pUserName) {
		__pUserName->RemoveAll(true);
		delete __pUserName;
	}

	if (__pDialogText) {
		delete __pDialogText;
	}
	if (__pDialogLabel) {
		__pDialogLabel->RemoveAll(true);
		delete __pDialogLabel;
	}
}

bool
UiDialogListItem::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

//	int height = rect.height;

/******************** TIME LABEL ************************/

	Bitmap *pRounder = null;

	if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
		if (this->GetDialog()->GetReadState() == 0 && this->GetDialog()->GetOut() == 0) {
			pRounder = Resources::getInstance().GetNormalUnreadRoundImageForm();
		} else {
			pRounder = Resources::getInstance().GetNormalRoundImageForm();
		}
	} else {
		pRounder = Resources::getInstance().GetSelectedRoundImageForm();
	}

	if (this->GetDialog()->GetUid() > isChatValue) {
		this->DrawRounders(canvas, rect, pRounder);
	} else {
		canvas.DrawBitmap(Rectangle(80 - 108/2, rect.height/2 - 108/2, 108, 108), *pRounder);
	}

	canvas.DrawText(drawTimePoint, *__pTimeLabel);

/************************* TITLE **********************/

	canvas.DrawText(drawUserNamePoint, *__pUserName);

/*********************** CHAT ICON ****************************/

	if (this->GetDialog()->GetUid() > isChatValue) {
		canvas.DrawBitmap(drawChatRect, *Resources::getInstance().GetGroupNormalIcon());
	}

/*********************** USER ICON ****************************/



/*********************** ONLINE STATUS ***********************/

	if (this->GetDialog()->GetIsOnline() == 1) {
		canvas.DrawBitmap( drawOnlineIconRect, *Resources::getInstance().GetOnlineIndicator());
	}

/*********************** DIALOG TEXT ************************/

	if (this->GetDialog()->GetOut() == 1) {
		Color *color = new Color(25, 31, 37, 255);

		Bitmap *userAvatar = Resources::getInstance().GetUserAvatar();

		if (userAvatar) {
			canvas.DrawBitmap(drawAvatarRect, *userAvatar);
		}

		Bitmap *pRounder = null;

		if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
			pRounder = Resources::getInstance().GetNormalRoundImageForm();
		} else {
			pRounder = Resources::getInstance().GetSelectedRoundImageForm();
		}

		canvas.DrawBitmap(drawAvatarRect, *pRounder);

		if (this->GetDialog()->GetReadState() == 0) {
			canvas.FillRectangle(*color, drawTextBgRect);
		}

		delete color;
	}

	//5 для красоты :/
	canvas.DrawText(drawDialogPoint, *__pDialogLabel);

	return true;
}

void
UiDialogListItem::DrawRounders(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Bitmap *bitmap) {
	if (this->__pDialog->GetUsers() && this->__pDialog->GetUsers()->GetCount() > 0) {

		int userCount = this->__pDialog->GetUsers()->GetCount();

		for (int index = 0; index < userCount; index ++) {
			Rectangle imageRect = Rectangle(0,0,0,0);

			float itemSize = 55;
			float centerX = 80;
			float centerY = rect.height/2;
			if (userCount >= 4) {

				switch(index) {
				case 0:
					imageRect = Rectangle(centerX - itemSize, centerY - itemSize, itemSize, itemSize);
					break;
				case 1:
					imageRect = Rectangle(centerX, centerY - itemSize, itemSize, itemSize);
					break;
				case 2:
					imageRect = Rectangle(centerX - itemSize, centerY, itemSize, itemSize);
					break;
				case 3:
					imageRect = Rectangle(centerX, centerY, itemSize, itemSize);
					break;
				default:
					break;
				}


			} else if (userCount == 3) {
				switch(index) {
				case 0:
					imageRect = Rectangle(centerX - itemSize, centerY - itemSize, itemSize, itemSize);
					break;
				case 1:
					imageRect = Rectangle(centerX, centerY - itemSize, itemSize, itemSize);
					break;
				case 2:
					imageRect = Rectangle(centerX - itemSize/2, centerY, itemSize, itemSize);
					break;
				default:
					break;
				}

			} else if (userCount == 2) {
				switch(index) {
				case 0:
					imageRect = Rectangle(centerX - itemSize, centerY - itemSize/2, itemSize, itemSize);
					break;
				case 1:
					imageRect = Rectangle(centerX, centerY - itemSize/2, itemSize, itemSize);
					break;

				default:
					break;
				}
			} else if (userCount == 1) {
				imageRect = Rectangle(80 - 108/2, rect.height/2 - 108/2, 108, 108);
			}

			canvas.DrawBitmap(imageRect, *bitmap);
		}
	} else {
		canvas.DrawBitmap( Rectangle(80 - 108/2, rect.height/2 - 108/2, 108, 108), *bitmap);
	}
}

void
UiDialogListItem::SetDialog(MDialog *pDialog) {
	result r;

	this->__pDialog = pDialog;

	int shift = 0;

	int width = this->__pDimension.width;
	int height = this->__pDimension.height;
	int halfHeight = height/2;

	int rightOffset = 15;

	int onlineStatusSize = 44;

	EnrichedText* pTimeLabel = null;
	TextElement* pTImeText = null;

	pTimeLabel = new EnrichedText();
	r = pTimeLabel->Construct(Dimension(140, 60));

	pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pTimeLabel->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
	pTimeLabel->SetTextAbbreviationEnabled(true);

	pTImeText = new TextElement();

	String *text = Util::formatDateN(this->__pDialog->GetDate());

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	r = pTImeText->Construct(text->GetPointer());


	GetErrorMessage(r);
	Color *timeColor = new Color(109, 110, 117, 255);
	pTImeText->SetTextColor(*timeColor);
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 28);
		pTImeText->SetFont(font);
	}

	pTimeLabel->Add(*pTImeText);
	delete timeColor;

	FloatDimension timeSize = pTimeLabel->GetTextExtentF();
	pTimeLabel->SetSize(timeSize);

	float xTimePosition = this->__pDimension.width - rightOffset - timeSize.width;
	float yTimePosition = (float)halfHeight - halfHeight/2  - (float)timeSize.height/2;

	this->__pTimeLabel = pTimeLabel;
	this->__pTimeText = pTImeText;

	drawTimePoint = Point(xTimePosition, yTimePosition);
//	delete text;
	/************************* TITLE **********************/

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	float leftOffset = 0;

	if (this->GetDialog()->GetUid() > isChatValue) {
		leftOffset = 50;
	}

	float titleWidth = width - baseListItemOffset - rightOffset - timeSize.width - leftOffset;

	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(titleWidth, 60));

	pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pUserName->SetTextAbbreviationEnabled(true);

	pUsetNameText = new TextElement();

	String *titleText;
	if (this->GetDialog()->GetUid() < isChatValue) {
		String *fullName = new String();
		if (this->GetDialog()->GetFirstName()) {
			fullName->Append(this->GetDialog()->GetFirstName()->GetPointer());
		}
		fullName->Append(L" ");
		if (this->GetDialog()->GetLastName()) {
			fullName->Append(this->GetDialog()->GetLastName()->GetPointer());
		}
		titleText = fullName;
	} else {
		if (this->GetDialog()->GetTitle()) {
			titleText = this->GetDialog()->GetTitle();
		} else {
			titleText = new String(L" ");
		}
	}
	r = pUsetNameText->Construct(titleText->GetPointer());

	Color *userNameColor = new Color(250, 250, 250, 255);
	pUsetNameText->SetTextColor(*userNameColor);
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 36);
		pUsetNameText->SetFont(font);
	}
	pUserName->Add(*pUsetNameText);
	delete userNameColor;

	FloatDimension size;
	int actualLength;
	pUserName->GetTextExtent(0, titleText->GetLength(), size, actualLength);

	if (size.height > 68) {
		size.height = 41;
	}

	if (size.width > titleWidth) {
		size.width = titleWidth;
	}

	pUserName->SetSize(size);

	float textPosition =  (float)halfHeight/2 -  (float)size.height/2;

	float titleOffset = baseListItemOffset;
	if (this->GetDialog()->GetUid() > isChatValue) {
		titleOffset += leftOffset;
	}

//	delete titleText;

	this->__pUserName = pUserName;
	this->__pUserNameText = pUsetNameText;

	this->drawUserNamePoint = Point(titleOffset, textPosition + shift);

	drawChatRect = Rectangle(
							baseListItemOffset,
							textPosition + shift + size.height - onlineStatusSize,
							onlineStatusSize,
							onlineStatusSize);

	float offlinePosition = (float)(textPosition + (float)size.height/2) - onlineStatusSize/2;
	drawOnlineIconRect = Rectangle(titleOffset + size.width, offlinePosition, onlineStatusSize, onlineStatusSize);

	/*********************** DIALOG TEXT ************************/

	EnrichedText* pDialogLabel = null;
	TextElement* pDialogText = null;

	int xOffset = baseListItemOffset;

	int textRightOffset = rightOffset;

	if (this->GetDialog()->GetOut() == 1) {
		xOffset = baseListItemOffset + 68;

		if (this->GetDialog()->GetReadState() == 0) {
			xOffset += 20;
			textRightOffset += 10;
		} else {
			xOffset += 20;
		}
	}

	pDialogLabel = new EnrichedText();
	r = pDialogLabel->Construct(Dimension(width - xOffset - textRightOffset, 44));

	pDialogLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pDialogLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pDialogLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pDialogLabel->SetTextAbbreviationEnabled(true);

	pDialogText = new TextElement();

	String *dialogText = this->__pDialog->GetText();

	if (!dialogText || (dialogText && dialogText->GetLength() == 0)) {
		String msgString(L" ");

		if (this->__pDialog->__attachmentCount > 0) {
			if (this->__pDialog->__attachmentCount == 1) {
				Application::GetInstance()->GetAppResource()->GetString(IDS_NOTIFY_ATTACHMENT, msgString);
			} else {
				Application::GetInstance()->GetAppResource()->GetString(IDS_NOTIFY_ATTACHMENTS, msgString);
			}
		} else if (this->__pDialog->__fwdCount > 0) {
			if (this->__pDialog->__fwdCount == 1) {
				Application::GetInstance()->GetAppResource()->GetString(IDS_NOTIFY_FWD, msgString);
			} else {
				Application::GetInstance()->GetAppResource()->GetString(IDS_NOTIFY_FWDS, msgString);
			}
		}

		dialogText = new String(msgString);
	}

	r = pDialogText->Construct(dialogText->GetPointer());
	GetErrorMessage(r);
	Color *textColor = new Color(86, 87, 93, 255);
	pDialogText->SetTextColor(*textColor);
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 34);
		pDialogText->SetFont(font);
	}
	pDialogLabel->Add(*pDialogText);
	delete textColor;

	FloatDimension textSize = pDialogLabel->GetTextExtentF();

	//AppLogDebug("%f :: %f :: %d", textSize.width, textSize.height, pDialogLabel->GetDisplayLineCount());

	if (textSize.height > 68) {
		textSize.height = 41;
	}

	pDialogLabel->SetSize(textSize);
	float yTextPosition = (float)halfHeight + (float)halfHeight/2 - (float)textSize.height/2;

	drawDialogPoint = Point(xOffset, yTextPosition - 5);

	drawAvatarRect = Rectangle(baseListItemOffset, halfHeight - 5, halfHeight, halfHeight);
	drawTextBgRect = Rectangle(
			xOffset - 10,
			yTextPosition - 10,
			width - xOffset - rightOffset + 10,
			textSize.height + 10);

	__pDialogLabel = pDialogLabel;
	this->__pDialogText = pDialogText;
}

MDialog *
UiDialogListItem::GetDialog() {
	return __pDialog;
}

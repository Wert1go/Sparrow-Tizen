/*
 * UiUserListItem.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: developer
 */

#include "UiUserListItem.h"

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>
#include "MUser.h"
#include "Util.h"

#include "Resources.h"

using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

static int baseListItemOffset = 150;

UiUserListItem::UiUserListItem() {
	__pUser = null;
	__pUserName = null;
	__pUserNameText = null;

	 __pContactName = null;
	__pContactText = null;
}

UiUserListItem::~UiUserListItem() {

	if (__pUserNameText) {
		delete __pUserNameText;
	}

	if (__pContactText) {
		delete __pContactText;
	}

	if (__pUserName) {
		__pUserName->RemoveAll(true);
		delete __pUserName;
	}

	if (__pContactName) {
		__pContactName->RemoveAll(true);
		delete __pContactName;
	}
}

void
UiUserListItem::SetUser(MUser *pUser) {
	if (!pUser) {
		return;
	}

	__pUser = pUser;

	__pUserName = null;
	__pUserNameText = null;

	result r = E_SUCCESS;

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	int rightOffset = 125;

	//WARNING
	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(720 - baseListItemOffset - rightOffset, 60));

	pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pUserName->SetTextAbbreviationEnabled(true);

	pUsetNameText = new TextElement();
	String *fullName = new String(L"");
	fullName->Append(this->GetUser()->GetFirstName()->GetPointer());
	fullName->Append(L" ");
	fullName->Append(this->GetUser()->GetLastName()->GetPointer());

	r = pUsetNameText->Construct(fullName->GetPointer());
	Color *userNameColor;

	if (pUser->__isContact == 1) {
		userNameColor = new Color(127, 127, 127, 255);
	} else {
		userNameColor = new Color(250, 250, 250, 255);
	}

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
	pUserName->GetTextExtent(0, fullName->GetLength(), size, actualLength);

	if (size.height > 68) {
		size.height = 41;
	}

	if (size.width > 720 - baseListItemOffset - rightOffset) {
		size.width = 720 - baseListItemOffset - rightOffset;
	}

	pUserName->SetSize(size);

	__pUserName = pUserName;
	__pUserNameText = pUsetNameText;

	if (pUser->__isContact == 1) {
		__pContactName = null;
		__pContactText = null;

		result r = E_SUCCESS;

		EnrichedText* pUserName = null;
		TextElement* pUsetNameText = null;

		int rightOffset = 125;

		//WARNING
		pUserName = new EnrichedText();
		r = pUserName->Construct(Dimension(720 - baseListItemOffset - rightOffset, 60));

		pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
		pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pUserName->SetTextAbbreviationEnabled(true);

		pUsetNameText = new TextElement();
		String *fullName = new String(pUser->__pContactName->GetPointer());

		r = pUsetNameText->Construct(fullName->GetPointer());
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
		pUserName->GetTextExtent(0, fullName->GetLength(), size, actualLength);

		if (size.height > 68) {
			size.height = 41;
		}

		if (size.width > 720 - baseListItemOffset - rightOffset) {
			size.width = 720 - baseListItemOffset - rightOffset;
		}

		pUserName->SetSize(size);

		__pContactName = pUserName;
		__pContactText = pUsetNameText;
	}
}

MUser *
UiUserListItem::GetUser() {
	return __pUser;
}

bool
UiUserListItem::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	int shift = 0;

//	int height = rect.height;

	//AppLog("UiUserListItem::OnDraw  %d", width);

	int rightOffset = 25;

	int onlineStatusSize = 44;

/******************** TIME LABEL ************************/

	Bitmap *pRounder = null;

	if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
		pRounder = Resources::getInstance().GetNormalRoundImageForm();
	} else {
		pRounder = Resources::getInstance().GetSelectedRoundImageForm();
	}

	canvas.DrawBitmap(Rectangle(80 - 108/2, rect.height/2 - 108/2, 108, 108), *pRounder);

	if (!__pUser) {
		return true;
	}

/************************* TITLE **********************/

	if (this->__pContactName) {
		if (this->__pUser->__isFriend) {
			float textPosition =  rect.height/2 - (rect.height/2)/2 -  (float)__pUserName->GetSize().height/2;
			float textPosition1 =  rect.height/2 + (rect.height/2)/2 -  (float)__pUserName->GetSize().height/2;

			canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *__pContactName);

			canvas.DrawText(FloatPoint(baseListItemOffset, textPosition1 + shift), *__pUserName);
		} else {
			float textPosition =  rect.height/2 -  (float)__pUserName->GetSize().height/2;
			canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *__pContactName);
		}
	} else {
		float textPosition =  rect.height/2 -  (float)__pUserName->GetSize().height/2;

		canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *__pUserName);
	}

/*********************** ONLINE STATUS ***********************/

	if (this->GetUser()->GetIsOnline() == 1) {
		float offlinePosition = rect.height/2 - onlineStatusSize/2;
		canvas.DrawBitmap(Rectangle(
				rect.width - rightOffset - onlineStatusSize,
				offlinePosition,
				onlineStatusSize,
				onlineStatusSize
				), *Resources::getInstance().GetOnlineIndicator());
	}

	return true;
}

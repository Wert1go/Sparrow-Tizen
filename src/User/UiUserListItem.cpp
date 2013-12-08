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

}

UiUserListItem::~UiUserListItem() {

	if (__pUserNameText) {
		delete __pUserNameText;
	}

	if (__pUserName) {
		__pUserName->RemoveAll(true);
		delete __pUserName;
	}
}

void
UiUserListItem::SetUser(MUser *pUser) {
	__pUser = pUser;

	__pUserName = null;
	__pUserNameText = null;

	result r = E_SUCCESS;

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	int rightOffset = 25;

	//WARNING
	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(720 - baseListItemOffset - rightOffset, 60));

	pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pUserName->SetTextAbbreviationEnabled(true);

	pUsetNameText = new TextElement();
	String *fullName = new String();
	fullName->Append(this->GetUser()->GetFirstName()->GetPointer());
	fullName->Append(L" ");
	fullName->Append(this->GetUser()->GetLastName()->GetPointer());

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

	pUserName->SetSize(size);

	__pUserName = pUserName;
	__pUserNameText = pUsetNameText;
}

MUser *
UiUserListItem::GetUser() {
	return __pUser;
}

bool
UiUserListItem::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	int shift = 0;

	int height = rect.height;

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


/************************* TITLE **********************/


	float textPosition =  rect.height/2 -  (float)__pUserName->GetSize().height/2;

	canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *__pUserName);


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

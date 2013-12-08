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

}

void
UiUserListItem::SetUser(MUser *pUser) {
	__pUser = pUser;
}

MUser *
UiUserListItem::GetUser() {
	return __pUser;
}

bool
UiUserListItem::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	result r = E_SUCCESS;

	int shift = 0;

	int width = rect.width;
	int height = rect.height;
	int halfHeight = height/2;

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

//	EnrichedText* pTimeLabel = null;
//	TextElement* pTImeText = null;
//
//	pTimeLabel = new EnrichedText();
//	r = pTimeLabel->Construct(Dimension(180, 60));
//
//	pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
//	pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
//	pTimeLabel->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
//	pTimeLabel->SetTextAbbreviationEnabled(true);
//
//	pTImeText = new TextElement();
//
//	String *text = Util::formatDateN(this->__pDialog->GetDate());
//
//	if (text->GetLength() == 0) {
//		text = new String(L" ");
//	}
//
//	r = pTImeText->Construct(text->GetPointer());
//
//	GetErrorMessage(r);
//	Color *timeColor = new Color(109, 110, 117, 255);
//	pTImeText->SetTextColor(*timeColor);
//	{
//		Font font;
//		font.Construct(FONT_STYLE_BOLD, 28);
//		pTImeText->SetFont(font);
//	}
//	pTimeLabel->Add(*pTImeText);
//	delete timeColor;
//
//	FloatDimension timeSize = pTimeLabel->GetTextExtentF();
//	pTimeLabel->SetSize(timeSize);
//
//	float xTimePosition = rect.width - rightOffset - timeSize.width;
//	float yTimePosition = (float)halfHeight - halfHeight/2  - (float)timeSize.height/2;
//
//	canvas.DrawText(FloatPoint(xTimePosition, yTimePosition), *pTimeLabel);

/************************* TITLE **********************/

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(rect.width - baseListItemOffset - rightOffset, 60));

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

	float textPosition =  rect.height/2 -  (float)size.height/2;//(float)halfHeight/2 -  (float)size.height/2;

	canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *pUserName);


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

/*********************** DIALOG TEXT ************************/

//	if (this->GetUser()->GetText() && this->GetUser()->GetText()->GetLenght()) {
//
//	}
//
//	EnrichedText* pDialogLabel = null;
//	TextElement* pDialogText = null;
//
//	int xOffset = baseListItemOffset;
//
//
//	pDialogLabel = new EnrichedText();
//	r = pDialogLabel->Construct(Dimension(width - xOffset - textRightOffset, 44));
//
//	pDialogLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
//	pDialogLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
//	pDialogLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
//	pDialogLabel->SetTextAbbreviationEnabled(true);
//
//	pDialogText = new TextElement();
//
//	String *dialogText = this->GetUser()->GetText();
//
//	if (dialogText->GetLength() == 0) {
//		dialogText = new String(L" ");
//	}
//
//	r = pDialogText->Construct(dialogText->GetPointer());
//	GetErrorMessage(r);
//	Color *textColor = new Color(86, 87, 93, 255);
//	pDialogText->SetTextColor(*textColor);
//	{
//		Font font;
//		font.Construct(FONT_STYLE_BOLD, 34);
//		pDialogText->SetFont(font);
//	}
//	pDialogLabel->Add(*pDialogText);
//	delete textColor;
//
//	FloatDimension textSize = pDialogLabel->GetTextExtentF();
//
//	//AppLogDebug("%f :: %f :: %d", textSize.width, textSize.height, pDialogLabel->GetDisplayLineCount());
//
//	if (textSize.height > 68) {
//		textSize.height = 41;
//	}
//
//	pDialogLabel->SetSize(textSize);
//	float yTextPosition = (float)halfHeight + (float)halfHeight/2 - (float)textSize.height/2;
//
//	//5 для красоты :/
//	canvas.DrawText(FloatPoint(xOffset, yTextPosition - 5), *pDialogLabel);

	return true;
}

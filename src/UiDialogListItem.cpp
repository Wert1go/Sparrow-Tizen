/*
 * UiDialogListItem.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#include "UiDialogListItem.h"

#include <FUi.h>
#include <FGraphics.h>
#include "MDialog.h"
#include "Util.h"

#include "Resources.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

static int baseListItemOffset = 150;

UiDialogListItem::UiDialogListItem() {
	// TODO Auto-generated constructor stub

}

UiDialogListItem::~UiDialogListItem() {
	// TODO Auto-generated destructor stub
}

bool
UiDialogListItem::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	result r = E_SUCCESS;

	int shift = 0;

	int width = rect.width;
	int height = rect.height;
	int halfHeight = height/2;

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(500, 60));

	pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pUserName->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pUserName->SetTextAbbreviationEnabled(true);

	pUsetNameText = new TextElement();
	String *fullName = new String();
	fullName->Append(this->GetDialog()->GetFirstName()->GetPointer());
	fullName->Append(L" ");
	fullName->Append(this->GetDialog()->GetLastName()->GetPointer());

	r = pUsetNameText->Construct(fullName->GetPointer());
	//delete fullName;

	pUsetNameText->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 36);
		pUsetNameText->SetFont(font);
	}
	pUserName->Add(*pUsetNameText);

	FloatDimension size;
	int actualLength;
	pUserName->GetTextExtent(0, fullName->GetLength(), size, actualLength);

	pUserName->SetSize(size);

	float textPosition =  (float)halfHeight/2 -  (float)size.height/2;

	canvas.DrawText(FloatPoint(baseListItemOffset, textPosition + shift), *pUserName);

	if (this->GetDialog()->GetIsOnline() == 1) {
		float offlinePosition = (float)(textPosition + (float)size.height/2) - 44/2;
		canvas.DrawBitmap(Rectangle(baseListItemOffset + size.width, offlinePosition, 44, 44), *Resources::getInstance().GetOnlineIndicator());
	}

/*********************** DIALOG TEXT ************************/

	EnrichedText* pDialogLabel = null;
	TextElement* pDialogText = null;

	pDialogLabel = new EnrichedText();
	r = pDialogLabel->Construct(Dimension(width - baseListItemOffset * 2, 44));

	pDialogLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pDialogLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pDialogLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pDialogLabel->SetTextAbbreviationEnabled(true);

	pDialogText = new TextElement();

	String *text = this->__pDialog->GetText();

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	r = pDialogText->Construct(text->GetPointer());
	GetErrorMessage(r);
	pDialogText->SetTextColor(Color::GetColor(COLOR_ID_GREY));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 34);
		pDialogText->SetFont(font);
	}
	pDialogLabel->Add(*pDialogText);

	int xOffset = baseListItemOffset;

	FloatDimension textSize = pDialogLabel->GetTextExtentF();

	if (this->GetDialog()->GetOut() == 1) {
		xOffset = baseListItemOffset + 50;
	}

	AppLogDebug("%f :: %f :: %d", textSize.width, textSize.height, pDialogLabel->GetDisplayLineCount());

	if (textSize.height > 68) {
		textSize.height = 41;
	}

	pDialogLabel->SetSize(textSize);
	float yTextPosition = (float)halfHeight + (float)halfHeight/2 - (float)textSize.height/2;

	canvas.DrawText(FloatPoint(xOffset, yTextPosition - 5), *pDialogLabel);

/******************** TIME LABEL ************************/

	EnrichedText* pTimeLabel = null;
	TextElement* pTImeText = null;

	pTimeLabel = new EnrichedText();
	r = pTimeLabel->Construct(Dimension(180, 60));

	pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pTimeLabel->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
	pTimeLabel->SetTextAbbreviationEnabled(true);

	pTImeText = new TextElement();

	text = Util::formatDateN(this->__pDialog->GetDate());

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	AppLogDebug("!!pre text %S", this->__pDialog->GetTitle()->GetPointer());
	r = pTImeText->Construct(text->GetPointer());
	AppLogDebug("!!pre text %S", this->__pDialog->GetTitle()->GetPointer());
	GetErrorMessage(r);
	pTImeText->SetTextColor(Color::GetColor(COLOR_ID_GREY));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 18);
		pTImeText->SetFont(font);
	}
	pTimeLabel->Add(*pTImeText);

	canvas.DrawText(FloatPoint(400, 10), *pTimeLabel);

	return true;
}

void
UiDialogListItem::SetDialog(MDialog *pDialog) {
	this->__pDialog = pDialog;
}

MDialog *
UiDialogListItem::GetDialog() {
	return __pDialog;
}

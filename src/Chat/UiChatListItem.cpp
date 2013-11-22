/*
 * UiChatListItem.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#include "UiChatListItem.h"
#include "Util.h"
#include "MMessage.h"
#include "MUser.h"
#include "MDialog.h"
#include "Resources.h"
#include "IImageDrawer.h"

using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

const int avatarSize = 55;

UiChatListItem::UiChatListItem() {
	__pMessage = null;
	__offset = 20;
	__sideOffset = __offset*3;
	__leftOffset = __sideOffset;
	__triangleWidth = 25;
	__triangleHeight = 32;
}

UiChatListItem::~UiChatListItem() {
	// TODO Auto-generated destructor stub
}

bool
UiChatListItem::OnDraw(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	if (LIST_ITEM_DRAWING_STATUS_NORMAL == status) {
		if (this->GetMessage()->GetReadState() == 0) {
			canvas.FillRectangle(Color(26, 31, 37, 255), rect);
		} else {
			canvas.FillRectangle(Color(8, 8, 8, 255), rect);
		}
	} else {
		canvas.FillRectangle(Color(20, 20, 20, 255), rect);
	}

	this->DrawBubble(canvas, rect, status);
	this->DrawMessage(canvas, rect, status);

	if (this->GetMessage()->GetOut() == 0 && this->__pDialog->GetUid() > isChatValue) {

		int uid = this->__pMessage->GetFromUid();
		String *url = null;

		for (int i = 0; i < this->__pDialog->GetUsers()->GetCount(); i++) {
			MUser *user = static_cast<MUser *> (this->__pDialog->GetUsers()->GetAt(i));

			if(user->GetUid() == uid) {
				url = user->GetPhoto();
				break;
			}
		}

		this->GetDrawer()->DrawImageFromUrlInRect(url, Rectangle(20, 30, avatarSize, avatarSize));
	}

	AppLog("DRAWEDDD");

	return true;
}

void
UiChatListItem::DrawBubble(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	FloatRectangle bubbleRect;

	Point triangleFirstPoint;
	Point triangleSecondPoint;
	Point triangleThirdPoint;

	Color normalColor;
	Color selectedColor;

	float width = __pBubbleDimension.width;
	float height = __pBubbleDimension.height;

	FloatDimension rounds = FloatDimension(10,10);

	float offset = __offset;
	float sideOffset = __sideOffset;
	float triangleWidth = __triangleWidth;

	if (this->GetMessage()->GetOut() == 1) {
		bubbleRect = FloatRectangle (
						rect.width - sideOffset - width,
						0 + offset,
						width,
						rect.height - offset*2);

		normalColor = Color(56, 79, 108, 255);
		selectedColor = Color(36, 54, 69, 255);

		triangleFirstPoint = Point(rect.width - sideOffset, rect.y + offset + 30);
		triangleSecondPoint = Point(rect.width - sideOffset + triangleWidth, rect.y + offset + 30);
		triangleThirdPoint = Point(rect.width - sideOffset, rect.y + offset + 30 + __triangleHeight);
	} else {
		bubbleRect = FloatRectangle (
								__leftOffset,
								0 + offset,
								width,
								height - offset*2);

		normalColor = Color(76, 109, 150, 255);
		selectedColor = Color(50, 255, 100, 255);

		triangleFirstPoint = Point(__leftOffset, rect.y + offset + 30);
		triangleSecondPoint = Point(__leftOffset - triangleWidth, rect.y + offset + 30);
		triangleThirdPoint = Point(__leftOffset, rect.y + offset +  30 + __triangleHeight);
	}

	if (status == LIST_ITEM_DRAWING_STATUS_PRESSED) {
		canvas.FillRoundRectangle(selectedColor, bubbleRect, rounds);
		canvas.FillTriangle(
				selectedColor,
				triangleFirstPoint,
				triangleSecondPoint,
				triangleThirdPoint );
	} else {
		canvas.FillRoundRectangle(normalColor, bubbleRect, rounds);
		canvas.FillTriangle(
				normalColor,
				triangleFirstPoint,
				triangleSecondPoint,
				triangleThirdPoint );
	}
}


void
UiChatListItem::DrawMessage(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	float width = __pBubbleDimension.width;
	float height = __pBubbleDimension.height;

	EnrichedText* pMessageLabel = null;
	TextElement* pMessageText = null;

	pMessageLabel = new EnrichedText();
	pMessageLabel->Construct(Dimension(limitSize, 480));

	pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
	pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

	pMessageText = new TextElement();

	String *text = this->GetMessage()->GetText();

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	pMessageText->Construct(*text);
	pMessageText->SetTextColor(Color(255, 255, 255, 255));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 36);
		pMessageText->SetFont(font);
	}

	pMessageLabel->Add(*pMessageText);

	Dimension resultSize;

	FloatDimension size;
	int actualLength;
	pMessageLabel->GetTextExtent(0, text->GetLength(), size, actualLength);

	if (size.width <= limitSize) {
		resultSize.width = size.width;
		resultSize.height = size.height;
	} else {
		Dimension normalSize = pMessageLabel->GetTextExtent();
		resultSize = normalSize;
	}

	pMessageLabel->SetSize(resultSize);

	Point drawPoint;

	if (this->GetMessage()->GetOut() == 1) {
		drawPoint = Point(rect.width - __sideOffset - width + __offset, height/2 - resultSize.height/2);
	} else {
		drawPoint = Point(__leftOffset + __offset, height/2 - resultSize.height/2);
	}

	canvas.DrawText(drawPoint, *pMessageLabel);

	EnrichedText* pTimeLabel = null;
	TextElement* pTImeText = null;

	pTimeLabel = new EnrichedText();
	pTimeLabel->Construct(Dimension(180, 60));

	pTimeLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
	pTimeLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pTimeLabel->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
	pTimeLabel->SetTextAbbreviationEnabled(true);

	pTImeText = new TextElement();

	text = Util::formatDateN(this->GetMessage()->GetDate());

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	pTImeText->Construct(text->GetPointer());

	pTImeText->SetTextColor( Color(109, 110, 117, 255));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 28);
		pTImeText->SetFont(font);
	}
	pTimeLabel->Add(*pTImeText);

	int actualLen;

	FloatDimension timeSize;
	pTimeLabel->GetTextExtent(0, text->GetLength(), timeSize, actualLen);
	pTimeLabel->SetSize(timeSize);

	Point datePoint;

	if (this->GetMessage()->GetOut() == 1) {
		datePoint = Point(
				rect.width - __sideOffset - width - timeSize.width - __offset,
				height - 136/2 - timeSize.height/2);
	} else {
		datePoint = Point(
				__leftOffset + width + __offset,
				height - 136/2 - timeSize.height/2);
	}

	if (this->GetMessage()->GetOut() == 1 && this->GetMessage()->GetDelivered() == 1 && this->GetMessage()->GetReadState() == 0) {
		FloatRectangle delivered = FloatRectangle(
				datePoint.x - timeSize.height - 5,
				datePoint.y,
				timeSize.height,
				timeSize.height
		);

		canvas.DrawBitmap(delivered, *Resources::getInstance().GetDeliveredIcon());
	}

	canvas.DrawText(datePoint, *pTimeLabel);
}

/************************** UTILS ******************************/

void
UiChatListItem::SetMessage(MMessage *pMessage) {
	__pMessage = pMessage;
}

MMessage *
UiChatListItem::GetMessage() {
	return __pMessage;
}

void
UiChatListItem::SetBubbleDimension(Dimension pDimension) {
	__pBubbleDimension = pDimension;
}

void
UiChatListItem::SetDialog(MDialog *pDialog) {
	__pDialog = pDialog;

	if (this->__pDialog->GetUid() > isChatValue) {
		__leftOffset += avatarSize;
	}
}

MDialog *
UiChatListItem::GetDialog() {
	return __pDialog;
}
void
UiChatListItem::SetDrawer(IImageDrawer *drawer) {
	__pImageDrawer = drawer;
}

IImageDrawer *
UiChatListItem::GetDrawer() {
	return __pImageDrawer;
}

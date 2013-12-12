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
#include "MAttachment.h"
#include "MGeo.h"

using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

const int avatarSize = 55;


UiChatListItem::UiChatListItem() {
	__textOffset = 0;
	__pMessage = null;
	__offset = 20;
	__sideOffset = __offset*3;
	__leftOffset = __sideOffset;
	__triangleWidth = 25;
	__triangleHeight = 32;

	__pMessageLabel = null;
	__pMessageText = null;
	__pTimeLabel = null;
	__pTimeText  = null;

}

UiChatListItem::~UiChatListItem() {
	__textOffset = 0;

	if (__pMessageText) {
		delete __pMessageText;
		__pMessageText = null;
	}

	if (__pTimeText) {
		delete __pTimeText;
		__pTimeText = null;
	}

	if (__pMessageLabel) {
		__pMessageLabel->RemoveAll(true);
		delete __pMessageLabel;
	}

	if (__pTimeLabel) {
		__pTimeLabel->RemoveAll(true);
		delete __pTimeLabel;
	}
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

	float drawOffset = __textOffset;

	if (this->GetMessage()->__pAttachments && this->GetMessage()->__pAttachments->GetCount() > 0) {

		for (int i = 0; i < this->GetMessage()->__pAttachments->GetCount(); i++) {
			MAttachment *attachment = static_cast<MAttachment *>( GetMessage()->__pAttachments->GetAt(i));

			Point drawPoint;
			float width = __pBubbleDimension.width;

			if (this->GetMessage()->GetOut() == 1) {
				drawPoint = Point(rect.width - __sideOffset - width + __offset, drawOffset);
			} else {
				drawPoint = Point(__leftOffset + __offset, drawOffset);
			}

			String *imgUrl = null;

			if (attachment->__pType->Equals(PHOTO, false)) {
				if (attachment->__pPhoto604) {
					imgUrl = attachment->__pPhoto604;
				} else {
					imgUrl = attachment->__pPhoto130;
				}
			} else if (attachment->__pType->Equals(VIDEO, false)) {
				if (attachment->__pVideoPhoto320) {
					imgUrl = attachment->__pVideoPhoto320;
				} else {
					imgUrl = attachment->__pPhoto130;
				}
			}

			this->GetDrawer()->DrawAttachmentFromUrlInRect(
					imgUrl,
					Rectangle(
							drawPoint.x,
							drawPoint.y,
							attachment->imageSize.x,
							attachment->imageSize.y),
					attachment
			);

			drawOffset += attachment->imageSize.y;

			if (i != this->GetMessage()->__pAttachments->GetCount() - 1) {
				drawOffset += msgImageOffset;
			}
		}
	}

	if (this->GetMessage()->__pGeo) {
		Point drawPoint;
		float width = __pBubbleDimension.width;

		if (this->GetMessage()->GetOut() == 1) {
			drawPoint = Point(rect.width - __sideOffset - width + __offset, drawOffset);
		} else {
			drawPoint = Point(__leftOffset + __offset, drawOffset);
		}

		this->GetDrawer()->DrawAttachmentFromUrlInRect(
					this->GetMessage()->__pGeo->GetImageUrl(),
							Rectangle(
									drawPoint.x,
									drawPoint.y + 20,
									this->GetMessage()->__pGeo->imageSize.x,
									this->GetMessage()->__pGeo->imageSize.y),
							this->GetMessage()->__pGeo
					);

		drawOffset += this->GetMessage()->__pGeo->imageSize.y;
	}

	if (this->GetMessage()->__pFwd) {
		for(int i = 0; i < this->GetMessage()->__pFwd->GetCount(); i++) {
			MMessage *pFwdMessage = static_cast<MMessage *>(this->GetMessage()->__pFwd->GetAt(i));

			Point drawPoint;
			float width = __pBubbleDimension.width;

			if (this->GetMessage()->GetOut() == 1) {
				drawPoint = Point(rect.width - __sideOffset - width + __offset, drawOffset);
			} else {
				drawPoint = Point(__leftOffset + __offset, drawOffset);
			}

			AppLog("drawPoint: %d, %d", drawPoint.x, drawPoint.y);

			pFwdMessage->__absolutePosition = drawPoint;

			this->GetDrawer()->DrawAttachmentFromUrlInRect(
								null,
								Rectangle(
										drawPoint.x,
										drawPoint.y + 20,
										pFwdMessage->imageSize.x,
										pFwdMessage->imageSize.y),
										pFwdMessage
						);

			drawOffset += pFwdMessage->imageSize.y;
		}
	}


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

	if (__pMessageLabel) {
		canvas.DrawText(__messageDrawPoint, *__pMessageLabel);
	}

	if (__pTimeLabel) {
		canvas.DrawText(__timeDrawPoint, *__pTimeLabel);


		if (this->GetMessage()->GetOut() == 1 && this->GetMessage()->GetDelivered() == 1 && this->GetMessage()->GetReadState() == 0) {
			FloatRectangle delivered = FloatRectangle(
					__timeDrawPoint.x - __timeSize.height - 5,
					__timeDrawPoint.y,
					__timeSize.height,
					__timeSize.height
			);

			canvas.DrawBitmap(delivered, *Resources::getInstance().GetDeliveredIcon());
		}
	}
}

/************************** UTILS ******************************/

void
UiChatListItem::Initalize() {
	float width = __pBubbleDimension.width;
	float height = __pBubbleDimension.height;

	String *text = this->GetMessage()->GetText();

	if (text->GetLength() != 0) {
		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(limitSize, 480));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

		pMessageText = new TextElement();

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

		Dimension normalSize = pMessageLabel->GetTextExtent();

		if (size.width <= limitSize && normalSize.height <= size.height) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		} else {
			resultSize = normalSize;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		float bubbleOffset = 40;

		if (this->GetMessage()->GetOut() == 1) {
			drawPoint = Point(__rect.width - __sideOffset - width + __offset, bubbleOffset);
		} else {
			drawPoint = Point(__leftOffset + __offset, bubbleOffset);
		}

		__textOffset = bubbleOffset + resultSize.height;

		__messageDrawPoint = drawPoint;
		__pMessageLabel = pMessageLabel;
		__pMessageText = pMessageText;

//		canvas.DrawText(drawPoint, *pMessageLabel);

	} else {
		__textOffset = 40;
	}

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
	__timeSize = timeSize;

	Point datePoint;

	if (this->GetMessage()->GetOut() == 1) {
		datePoint = Point(
				__rect.width - __sideOffset - width - timeSize.width - __offset,
				height - 136/2 - timeSize.height/2);
	} else {
		datePoint = Point(
				__leftOffset + width + __offset,
				height - 136/2 - timeSize.height/2);
	}

	__pTimeLabel = pTimeLabel;
	__pTimeText = pTImeText;
	__timeDrawPoint = datePoint;

//	canvas.DrawText(datePoint, *pTimeLabel);
//	delete pTImeText;
}

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

	if (!this->__pDialog) {
		AppLog("HOLLY FUCK!");
	}

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

/*
 * UiAttachmentView.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#include "UiAttachmentView.h"

#include <FUi.h>
#include <FGraphics.h>
#include "MAttachment.h"
#include "Resources.h"
#include "MGeo.h"
#include "MUser.h"
#include "IImageDrawer.h"
#include "Util.h"
#include "MMessage.h"

#include "Helper.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;


UiAttachmentView::UiAttachmentView() {
	__pBitmapImage = null;
	__pAttachment = null;

	__pTitleLabel = null;
	__pTitleText = null;

	__pDurationLabel = null;
	__pDurationText = null;

	__pTextLabel = null;
	__pTextText = null;

	__playing = false;
	__pImageDrawer = null;
}

UiAttachmentView::~UiAttachmentView() {
	AppLog("UiAttachmentView::~UiAttachmentView");
	SAFE_DELETE(__pBitmapImage);
	SAFE_DELETE(__pTitleText);
	SAFE_DELETE(__pDurationText);
	SAFE_DELETE(__pTextText);

	if (__pTitleLabel) {
		__pTitleLabel->RemoveAll(true);
		delete __pTitleLabel;
	}

	if (__pDurationLabel) {
		__pDurationLabel->RemoveAll(true);
		delete __pDurationLabel;
	}

	if (__pTextLabel) {
		__pTextLabel->RemoveAll(true);
		delete __pTextLabel;
	}
}

void
UiAttachmentView::SetBitmap(Bitmap *pBitmapImage) {
	SAFE_DELETE(__pBitmapImage);

	__pBitmapImage = pBitmapImage;
}

bool
UiAttachmentView::OnDraw(
		Tizen::Graphics::Canvas &canvas,
		const Tizen::Graphics::Rectangle &rect,
		Tizen::Ui::Controls::ListItemDrawingStatus status) {

	result r;

	if (!__pAttachment) {
		return true;
	}

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	}

	if (__pAttachment->__pType->Equals(VIDEO, false)) {

/*************************** VIDEO ******************************************/

		canvas.DrawBitmap(
				Rectangle(__pAttachment->imageSize.x/2 - 94/2, __pAttachment->imageSize.y/2 - 94/2, 94, 94),
				*Resources::getInstance().GetVideoPlayIcon());

		canvas.FillRectangle(Color(0, 0, 0, 150),
				Rectangle(0, __pAttachment->imageSize.y - 60, __pAttachment->imageSize.x, 60));

		if (__pTitleLabel && __pDurationLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
			canvas.DrawText(__durationDrawPoint, *__pDurationLabel);
		}

	} else if (__pAttachment->__pType->Equals(AUDIO, false)) {

/*************************** AUDIO ******************************************/

		Bitmap *pButton = null;

		if (this->__playing) {
			if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
				pButton = Resources::getInstance().GetAudioPauseIcon();
			} else {
				pButton = Resources::getInstance().GetAudioPausePressedIcon();
			}
		} else {
			if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
				pButton = Resources::getInstance().GetAudioPlayIcon();
			} else {
				pButton = Resources::getInstance().GetAudioPlayPressedIcon();
			}
		}

		canvas.DrawBitmap(Rectangle(5, rect.height/2 - 71/2, 71, 71), *pButton);

		if (__pTitleLabel && __pDurationLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
			canvas.DrawText(__durationDrawPoint, *__pDurationLabel);
		}


	} else if (__pAttachment->__pType->Equals(DOC, false)) {

/*************************** DOC ******************************************/

		canvas.FillRoundRectangle(Color(0, 0, 0, 100), rect, Dimension(8, 8));
		canvas.DrawBitmap(Rectangle(5, rect.height/2 - 72/2, 72, 72), *Resources::getInstance().GetDocumentIcon());

		if (__pTitleLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
		}
	} else if (__pAttachment->__pType->Equals(POINT, false)) {

/*************************** MAP ******************************************/

		if (__pTitleLabel) {
			canvas.FillRectangle(Color(0, 0, 0, 150), Rectangle(0, 0, __pAttachment->imageSize.x, 60));
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
		}

	} else if (__pAttachment->__pType->Equals(FWD, false)) {

/*************************** FWD ******************************************/

		canvas.FillRectangle(Color(86, 156, 218, 255), Rectangle(5, 0, 5, rect.height));

		if (__pImageDrawer && __pAttachment->__pUser) {
			__pImageDrawer->DrawImageFromUrlInRect(__pAttachment->__pUser->GetPhoto(),
					Rectangle(__pAttachment->__absolutePosition.x + 20, __pAttachment->__absolutePosition.y, 72, 72));
		}

		float drawOffset = 80;

		if (__pTitleLabel && __pDurationLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
			canvas.DrawText(__durationDrawPoint, *__pDurationLabel);

			if (__pTextLabel) {
				canvas.DrawText(__textDrawPoint, *__pTextLabel);
				drawOffset += __pTextLabel->GetSize().height;
			}
		}

		if (__pAttachment->__pAttachments && __pAttachment->__pAttachments->GetCount() > 0) {

			for (int i = 0; i < __pAttachment->__pAttachments->GetCount(); i++) {
				MAttachment *attachment = static_cast<MAttachment *>( __pAttachment->__pAttachments->GetAt(i));

				Point drawPoint = Point(
						__pAttachment->__absolutePosition.x + 20,
						__pAttachment->__absolutePosition.y + drawOffset);


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

				if (__pImageDrawer) {
					__pImageDrawer->DrawAttachmentFromUrlInRect(
							imgUrl,
							Rectangle(
									drawPoint.x,
									drawPoint.y,
									attachment->imageSize.x,
									attachment->imageSize.y),
							attachment
					);

					drawOffset += attachment->imageSize.y;

					if (i != __pAttachment->__pAttachments->GetCount() - 1) {
						drawOffset += 20;
					}
				}
			}

			if (__pImageDrawer && this->__pAttachment->__pGeo) {
				drawOffset += 20;
			}
		}

		if (__pTitleLabel && __pDurationLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
			canvas.DrawText(__durationDrawPoint, *__pDurationLabel);
		}

		if (__pImageDrawer && this->__pAttachment->__pGeo) {

					__pImageDrawer->DrawAttachmentFromUrlInRect(
							this->__pAttachment->__pGeo->GetImageUrl(),
							Rectangle(
									__pAttachment->__absolutePosition.x + 20,
									__pAttachment->__absolutePosition.y + drawOffset,
									this->__pAttachment->__pGeo->imageSize.x,
									this->__pAttachment->__pGeo->imageSize.y),
							this->__pAttachment->__pGeo);

			drawOffset += this->__pAttachment->__pGeo->imageSize.y;
		}

		if (this->__pAttachment->__pFwd && this->__pAttachment->__pFwd->GetCount() > 0) {
			for(int i = 0; i < this->__pAttachment->__pFwd->GetCount(); i++) {
				MMessage *pFwdMessage = static_cast<MMessage *>(this->__pAttachment->__pFwd->GetAt(i));

				if (pFwdMessage->imageSize.x == 0 && pFwdMessage->imageSize.y == 0) {
					continue;
				}

				Point drawPoint = Point(__pAttachment->__absolutePosition.x + 40, __pAttachment->__absolutePosition.y + drawOffset);

				pFwdMessage->__absolutePosition = drawPoint;

				__pImageDrawer->DrawAttachmentFromUrlInRect(
					null,
					Rectangle(
							drawPoint.x,
							drawPoint.y,
							pFwdMessage->imageSize.x,
							pFwdMessage->imageSize.y),
							pFwdMessage);

				drawOffset += pFwdMessage->imageSize.y;
			}

		}


	}

//	if (status == LIST_ITEM_DRAWING_STATUS_PRESSED) {
//		canvas.FillRoundRectangle(Color(0, 0, 0, 80), rect, Dimension(8, 8));
//	}

//	AppLog("OnDraw::END");
	return true;
}

void
UiAttachmentView::SetAttachment(MAttachment *pAttachment) {
//	AppLog("++++++++++++++++++++++++++++++++++++++++++++++++ 1 : %f %f ", pAttachment->imageSize.x, pAttachment->imageSize.y);
	if (pAttachment->__nesting >= 5) {
		return;
	}
//	AppLog("++++++++++++++++++++++++++++++++++++++++++++++++ 2");
	__pAttachment = pAttachment;
//	AppLog("UiAttachmentView::SetAttachment %S", __pAttachment->__pType->GetPointer());

	int width = __pAttachment->imageSize.x;
	int height = __pAttachment->imageSize.y;

	int labelHeight = 60;
	int titleHeight = 54;
	int durationLabelSize = 80;
	int offset = 10;

	int docOffset = 80;
	int audioOffset = 85;

	int audioHeight = height/2;
//	AppLog("!!!UiAttachmentView::SetAttachment");

	if (__pAttachment->__pType->Equals(VIDEO, false)) {
		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(width - durationLabelSize - offset, titleHeight));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pMessageLabel->SetTextAbbreviationEnabled(true);

		String *pTitleString = null;

		if (__pAttachment->__pTitle) {
			pTitleString = new String(__pAttachment->__pTitle->GetPointer());
		} else {
			pTitleString = new String(L"");
		}

		pMessageText = new TextElement();
		pMessageText->Construct(*pTitleString);
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 22);
			pMessageText->SetFont(font);
		}

		pMessageLabel->Add(*pMessageText);

		Dimension resultSize;

		FloatDimension size;
		int actualLength;
		pMessageLabel->GetTextExtent(0, pTitleString->GetLength(), size, actualLength);

		if (size.width <= width - durationLabelSize) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		}

		if (resultSize.height == 0) {
			resultSize.height = titleHeight;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		drawPoint = Point(offset, height - labelHeight/2 - resultSize.height/2);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;

/************** DURATION ***************/

		EnrichedText* pDurationLabel = null;
		TextElement* pDurationText = null;

		pDurationLabel = new EnrichedText();
		pDurationLabel->Construct(Dimension(durationLabelSize, 50));

		pDurationLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_RIGHT);
		pDurationLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pDurationLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

		pDurationText = new TextElement();

		String *pDuration = new String(L"");

		int hours = (int)(__pAttachment->__duration/3600);
		int minutes = (int)(__pAttachment->__duration/60);
		int seconds = __pAttachment->__duration % 60;

		String hoursString(L"");
		String minutesString;
		String secondsString;

		if (hours != 0) {
			hoursString.Format(10, L"%d:", hours);
		}

		if (minutes < 10) {
			minutesString.Format(10, L"0%d", minutes);
		} else {
			minutesString.Format(10, L"%d", minutes);
		}

		if (seconds < 10) {
			secondsString.Format(10, L"0%d", seconds);
		} else {
			secondsString.Format(10, L"%d", seconds);
		}

		if (hoursString.GetLength() != 0) {
			pDuration->Append(hoursString);
		}

		pDuration->Append(minutesString);
		pDuration->Append(L":");
		pDuration->Append(secondsString);

		pDurationText->Construct(*pDuration);


		pDurationText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 22);
			pDurationText->SetFont(font);
		}

		pDurationLabel->Add(*pDurationText);

		Dimension resultSizeDuration;
		size = FloatDimension(0,0);
		actualLength = 0;

		pDurationLabel->GetTextExtent(0, pDuration->GetLength(), size, actualLength);

		if (size.width <= durationLabelSize) {
			resultSizeDuration.width = size.width;
			resultSizeDuration.height = size.height;
		}

		pDurationLabel->SetSize(resultSizeDuration);

		drawPoint = Point(width - resultSizeDuration.width - 5, height - labelHeight/2 - resultSizeDuration.height/2);

		__durationDrawPoint = drawPoint;
		__pDurationLabel = pDurationLabel;
		__pDurationText = pDurationText;

		delete pDuration;
		delete pTitleString;
	} else if (__pAttachment->__pType->Equals(DOC, false)){
		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(width - docOffset - offset, height));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pMessageLabel->SetTextAbbreviationEnabled(true);

		String *pTitleString = null;

		if (__pAttachment->__pTitle) {
			pTitleString = new String(__pAttachment->__pTitle->GetPointer());
		} else {
			pTitleString = new String(L"");
		}

		pMessageText = new TextElement();
		pMessageText->Construct(*pTitleString);
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 22);
			pMessageText->SetFont(font);
		}

		pMessageLabel->Add(*pMessageText);

		Dimension resultSize;

		FloatDimension size;
		int actualLength;
		pMessageLabel->GetTextExtent(0, pTitleString->GetLength(), size, actualLength);

		if (size.width <= width - durationLabelSize) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		}

		if (resultSize.height == 0) {
			resultSize.height = titleHeight;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		drawPoint = Point(docOffset, height/2 - resultSize.height/2);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;

		delete pTitleString;

	} else if (__pAttachment->__pType->Equals(AUDIO, false)){
		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(width - audioOffset - offset, audioHeight));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pMessageLabel->SetTextAbbreviationEnabled(true);

		String *pTitleString = null;

		if (__pAttachment->__pArtist) {
			pTitleString = new String(__pAttachment->__pArtist->GetPointer());
		} else {
			pTitleString = new String(L"");
		}

		pMessageText = new TextElement();
		pMessageText->Construct(*pTitleString);
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 32);
			pMessageText->SetFont(font);
		}

		pMessageLabel->Add(*pMessageText);

		Dimension resultSize;

		FloatDimension size;
		int actualLength;
		pMessageLabel->GetTextExtent(0, pTitleString->GetLength(), size, actualLength);

		if (size.width <= width - durationLabelSize) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		}

		if (resultSize.height == 0) {
			resultSize.height = audioHeight;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		drawPoint = Point(offset + audioOffset, audioHeight/2 - resultSize.height/2);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;

/************** SUBTITLE ***************/

		EnrichedText* pDurationLabel = null;
		TextElement* pDurationText = null;

		pDurationLabel = new EnrichedText();
		pDurationLabel->Construct(Dimension(width - audioOffset - offset, audioHeight));

		pDurationLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pDurationLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pDurationLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pDurationLabel->SetTextAbbreviationEnabled(true);

		pDurationText = new TextElement();

		String *pSongString = null;

		if (this->__pAttachment->__pTitle) {
			pSongString = new String(this->__pAttachment->__pTitle->GetPointer());
		} else {
			pSongString = new String(L"");
		}

		pDurationText->Construct(*pSongString);

		pDurationText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 26);
			pDurationText->SetFont(font);
		}

		pDurationLabel->Add(*pDurationText);

		Dimension resultSizeDuration;
		size = FloatDimension(0,0);
		actualLength = 0;

		pDurationLabel->GetTextExtent(0, pSongString->GetLength(), size, actualLength);

		if (size.width <= durationLabelSize) {
			resultSizeDuration.width = size.width;
			resultSizeDuration.height = size.height;
		}
		if (resultSizeDuration.height == 0) {
			resultSizeDuration.height = audioHeight;
		}

		pDurationLabel->SetSize(resultSizeDuration);

		drawPoint = Point(offset + audioOffset, audioHeight + audioHeight/2 - resultSize.height/2);

		__durationDrawPoint = drawPoint;
		__pDurationLabel = pDurationLabel;
		__pDurationText = pDurationText;

		delete pSongString;
		delete pTitleString;
	} else if (__pAttachment->__pType->Equals("point", false)) {

		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(width - durationLabelSize - offset, titleHeight));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pMessageLabel->SetTextAbbreviationEnabled(true);

		String *pTitleString = null;

		if (__pAttachment->__pTitle && __pAttachment->__pTitle->GetLength() > 0) {
			pTitleString = new String(__pAttachment->__pTitle->GetPointer());
		} else {
			return;
		}

		pMessageText = new TextElement();
		pMessageText->Construct(*pTitleString);
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 22);
			pMessageText->SetFont(font);
		}

		pMessageLabel->Add(*pMessageText);

		Dimension resultSize;

		FloatDimension size;
		int actualLength;
		pMessageLabel->GetTextExtent(0, pTitleString->GetLength(), size, actualLength);

		if (size.width <= width - durationLabelSize) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		}

		if (resultSize.height == 0) {
			resultSize.height = titleHeight;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		drawPoint = Point(offset,  labelHeight/2 - resultSize.height/2);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;

	}  else if (__pAttachment->__pType->Equals(FWD, false)) {
		if (!this->__pAttachment->__pUser) {
			return;
		}

		int infoHeight = 72;
		int imageOffset = 100;

		EnrichedText* pMessageLabel = null;
		TextElement* pMessageText = null;

		pMessageLabel = new EnrichedText();
		pMessageLabel->Construct(Dimension(width - offset * 2 - imageOffset, infoHeight/2));

		pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pMessageLabel->SetTextAbbreviationEnabled(true);

		String *pTitleString = null;

		if (__pAttachment->__pUser->GetFirstName()) {
			pTitleString = new String(__pAttachment->__pUser->GetFirstName()->GetPointer());
		} else {
			pTitleString = new String(L"");
		}

		pMessageText = new TextElement();
		pMessageText->Construct(*pTitleString);
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 32);
			pMessageText->SetFont(font);
		}

		pMessageLabel->Add(*pMessageText);

		Dimension resultSize;

		FloatDimension size;
		int actualLength;
		pMessageLabel->GetTextExtent(0, pTitleString->GetLength(), size, actualLength);

		if (size.width <= width - durationLabelSize) {
			resultSize.width = size.width;
			resultSize.height = size.height;
		}

		if (resultSize.height == 0) {
			resultSize.height = audioHeight;
		}

		pMessageLabel->SetSize(resultSize);

		Point drawPoint;

		drawPoint = Point(offset + imageOffset, 0);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;

/************** SUBTITLE ***************/
		EnrichedText* pDurationLabel = null;
		TextElement* pDurationText = null;

		pDurationLabel = new EnrichedText();
		pDurationLabel->Construct(Dimension(width - offset * 2 - imageOffset, infoHeight/2));

		pDurationLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pDurationLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pDurationLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
		pDurationLabel->SetTextAbbreviationEnabled(true);

		pDurationText = new TextElement();

		String *pSongString = Util::formatDateN(this->__pAttachment->__date);

		pDurationText->Construct(*pSongString);

		pDurationText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 26);
			pDurationText->SetFont(font);
		}

		pDurationLabel->Add(*pDurationText);

		Dimension resultSizeDuration;
		size = FloatDimension(0,0);
		actualLength = 0;

		pDurationLabel->GetTextExtent(0, pSongString->GetLength(), size, actualLength);

		if (size.width <= durationLabelSize) {
			resultSizeDuration.width = size.width;
			resultSizeDuration.height = size.height;
		}
		if (resultSizeDuration.height == 0) {
			resultSizeDuration.height = audioHeight;
		}

		pDurationLabel->SetSize(resultSizeDuration);

		drawPoint = Point(offset + imageOffset, + infoHeight/2);

		__durationDrawPoint = drawPoint;
		__pDurationLabel = pDurationLabel;
		__pDurationText = pDurationText;

		delete pSongString;
		delete pTitleString;


/******************* MESSAGE *************************/
		int limitSize = 420;
		String *text = this->__pAttachment->__text;


		if (!text || text->GetLength() == 0) {
			return;
		}

		EnrichedText* pTexLabel = null;
		TextElement* pTextText = null;

		pTexLabel = new EnrichedText();

		int textOffset = 0;
		if (this->__pAttachment->cut != 0) {
			textOffset = this->__pAttachment->cut;
		} else {
			textOffset = offset * 2;
		}
		pTexLabel->Construct(Dimension(limitSize - textOffset, 480));

		pTexLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
		pTexLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
		pTexLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);

		pTextText = new TextElement();

		pTextText->Construct(*text);
		pTextText->SetTextColor(Color(255, 255, 255, 255));
		{
			Font font;
			font.Construct(FONT_STYLE_BOLD, 36);
			pTextText->SetFont(font);
		}

		pTexLabel->Add(*pTextText);

		Dimension textResultSize;

		FloatDimension textSize;
		int textActualLength;
		pTexLabel->GetTextExtent(0, text->GetLength(), textSize, textActualLength);

		Dimension normalSize = pTexLabel->GetTextExtent();

		if (textSize.width <= limitSize && normalSize.height <= textSize.height) {
			textResultSize.width = textSize.width;
			textResultSize.height = textSize.height;
		} else {
			textResultSize = normalSize;
		}

		pTexLabel->SetSize(textResultSize);

		Point textDrawPoint;

		textDrawPoint = Point(offset * 2, infoHeight + 5);
		__textDrawPoint = textDrawPoint;
		__pTextLabel = pTexLabel;
		__pTextText = pTextText;
	}
}

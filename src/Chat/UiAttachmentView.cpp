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

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;


UiAttachmentView::UiAttachmentView() {
	__pBitmapImage = null;
	__pAttachment = null;

	__pTitleLabel = null;
	__pTitleText = null;

	__pDurationLabel = null;
	__pDurationText = null;

	__playing = false;
	__pImageDrawer = null;
}

UiAttachmentView::~UiAttachmentView() {
//	AppLog("UiAttachmentView::~UiAttachmentView");
	if (__pBitmapImage) {
		delete __pBitmapImage;
	}

	if (__pTitleText) {
		delete __pTitleText;
	}

	if (__pDurationText) {
		delete __pDurationText;
	}

	if (__pTitleLabel) {
		__pTitleLabel->RemoveAll(true);
		delete __pTitleLabel;
	}

	if (__pDurationLabel) {
		__pDurationLabel->RemoveAll(true);
		delete __pDurationLabel;
	}
}

bool
UiAttachmentView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	AppLog("UiAttachmentView %d, %d", rect.x, rect.y);

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	}

	if (__pAttachment->__pType->Equals(VIDEO, false)) {
		canvas.DrawBitmap(Rectangle(320/2 - 94/2, 240/2 - 94/2, 94, 94), *Resources::getInstance().GetVideoPlayIcon());

		canvas.FillRectangle(Color(0, 0, 0, 150), Rectangle(0, 240 - 60, 320, 60));

		if (__pTitleLabel && __pDurationLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
			canvas.DrawText(__durationDrawPoint, *__pDurationLabel);
		}

	} else if (__pAttachment->__pType->Equals(AUDIO, false)) {

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
		canvas.FillRoundRectangle(Color(0, 0, 0, 100), rect, Dimension(8, 8));
		canvas.DrawBitmap(Rectangle(5, rect.height/2 - 72/2, 72, 72), *Resources::getInstance().GetDocumentIcon());

		if (__pTitleLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
		}
	} else if (__pAttachment->__pType->Equals(L"point", false)) {
		canvas.FillRectangle(Color(0, 0, 0, 150), Rectangle(0, 0, 320, 60));

		if (__pTitleLabel) {
			canvas.DrawText(__titleDrawPoint, *__pTitleLabel);
		}
	} else if (__pAttachment->__pType->Equals(L"fwd", false)) {
		canvas.FillRectangle(Color(86, 156, 218, 255), Rectangle(10, 0, 5, rect.height - 10));

		if (__pImageDrawer && __pAttachment->__pUser) {
			AppLog("%d, %d", rect.x, rect.y);
			__pImageDrawer->DrawImageFromUrlInRect(__pAttachment->__pUser->GetPhoto(),
					Rectangle(__pAttachment->__absolutePosition.x + 10, __pAttachment->__absolutePosition.y + 10, 72, 72));
		}

		float drawOffset = 80;

		if (__pAttachment->__pAttachments && __pAttachment->__pAttachments->GetCount() > 0) {

			for (int i = 0; i < __pAttachment->__pAttachments->GetCount(); i++) {
				MAttachment *attachment = static_cast<MAttachment *>( __pAttachment->__pAttachments->GetAt(i));

				Point drawPoint = Point(__pAttachment->__absolutePosition.x + 20, __pAttachment->__absolutePosition.y + drawOffset);


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
						drawOffset += 30;
					}
				}
			}
		}
	}

	if (status == LIST_ITEM_DRAWING_STATUS_PRESSED) {
		canvas.FillRoundRectangle(Color(0, 0, 0, 80), rect, Dimension(8, 8));
	}

//	AppLog("OnDraw::END");
	return true;
}

void
UiAttachmentView::SetAttachment(MAttachment *pAttachment) {
	__pAttachment = pAttachment;

	AppLog("UiAttachmentView::SetAttachment %S", __pAttachment->__pType->GetPointer());

	int width = __pAttachment->imageSize.x;
	int height = __pAttachment->imageSize.y;

	int labelHeight = 60;
	int titleHeight = 54;
	int durationLabelSize = 80;
	int offset = 10;

	int docOffset = 80;
	int audioOffset = 85;

	int audioHeight = height/2;
	AppLog("!!!UiAttachmentView::SetAttachment");

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

		drawPoint = Point(offset,  labelHeight/2 - resultSize.height/2);

		__titleDrawPoint = drawPoint;
		__pTitleLabel = pMessageLabel;
		__pTitleText = pMessageText;
	}
}

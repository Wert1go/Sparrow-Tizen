/*
 * AttachmentItem.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#include "AttachmentItem.h"
#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>
#include "Resources.h"
#include "IAttachmentItemListener.h"
#include "MAttachment.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Media;
using namespace Tizen::Base::Utility;

//добавить маску для предпросмотра изображения

static int iconSize = 72;
const int deleteButton = 345;
const int iconTopOffset = 10;

AttachmentItem::AttachmentItem() {

	__pAttachment = null;
	__pIcon = null;
	__pImage = null;
	__pItemEventListener = null;
	__index = 0;
	__progress = 0;
	__ready = false;

	__pTitleLabel = null;
	__pTitleText = null;
}

AttachmentItem::~AttachmentItem() {
	if (__pIcon) {
		delete __pIcon;
	}

	if (this->__pTitleText) {
		delete this->__pTitleText;
	}

	if (this->__pTitleLabel) {
		this->__pTitleLabel->RemoveAll(true);
		delete this->__pTitleLabel;
	}

	if (__pImage) {
		delete __pImage;
	}

	__pItemEventListener = null;
}

bool
AttachmentItem::Initialize(void) {
	result r = Construct(Rectangle(0, 0, 10, 10));
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	AppLog("Initialize");

	this->__pButton = new (std::nothrow) Button();
	__pButton->Construct(Rectangle(0,0,10,10));
	__pButton->SetNormalBackgroundBitmap(*Resources::getInstance().LoadBitmapNamed(L"banner_close_normal.png"));
	__pButton->SetHighlightedBackgroundBitmap(*Resources::getInstance().LoadBitmapNamed(L"banner_close_press.png"));
	__pButton->SetPressedBackgroundBitmap(*Resources::getInstance().LoadBitmapNamed(L"banner_close_press.png"));
	this->__pButton->SetActionId(deleteButton);
	this->__pButton->AddActionEventListener(*this);

	this->AddControl(__pButton);

	__pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_photo.png");

	String *fileName = null;

	if (this->__pAttachment->__id != 0) {
		this->__ready = true;
	}

	if (this->__pAttachment && this->__pAttachment->__pFilePath) {

		Image image;
		image.Construct();
		String filepath = *this->__pAttachment->__pFilePath;

		__pImage = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

		String tempPath;
		String urlDelim(L"/");
		StringTokenizer stringTokenizer(this->__pAttachment->__pFilePath->GetPointer(), urlDelim);

		while (stringTokenizer.HasMoreTokens())
		{
			stringTokenizer.GetNextToken(tempPath);
		}

		fileName = new String(tempPath);
	}

	if (!fileName) {
		fileName = new String(L"");
	}

	String *text = new String(*fileName);

	EnrichedText* pMessageLabel = null;
	TextElement* pMessageText = null;

	int width = 170;
	int height = 170;

	pMessageLabel = new EnrichedText();
	pMessageLabel->Construct(Dimension(
			width - 20,
			height - (iconTopOffset * 2 + 15 + iconSize))
			);

	pMessageLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
	pMessageLabel->SetVerticalAlignment(TEXT_ALIGNMENT_MIDDLE);
	pMessageLabel->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pMessageLabel->SetTextAbbreviationEnabled(true);

	pMessageText = new TextElement();

	pMessageText->Construct(*text);
	if (this->__ready) {
		pMessageText->SetTextColor(Color(255, 255, 255, 255));
	} else {
		pMessageText->SetTextColor(Color(90, 94, 98, 255));
	}

	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 22);
		pMessageText->SetFont(font);
	}

	pMessageLabel->Add(*pMessageText);

	Dimension resultSize;

	FloatDimension size;
	int actualLength;
	pMessageLabel->GetTextExtent(0, text->GetLength(), size, actualLength);

	if (size.width <= width - 30) {
		resultSize.width = width - 20;
		resultSize.height = size.height;
		AppLog("1");
	} else {
		Dimension normalSize = pMessageLabel->GetTextExtent();
		resultSize.height = normalSize.height;
		resultSize.width = width - 20;
	}


	if (resultSize.height >pMessageLabel->GetSize().height) {
		resultSize.height = height - (iconTopOffset * 2 + 15 + iconSize);
	}
	AppLog("test 3");
	pMessageLabel->SetSize(resultSize);

	this->__pTitleLabel = pMessageLabel;
	this->__pTitleText = pMessageText;

	return true;
}

result
AttachmentItem::OnInitializing(void) {
	result r = E_SUCCESS;


	AppLog("OnInitializing");
	return r;
}

result
AttachmentItem::OnTerminating(void) {
	result r = E_SUCCESS;
	return r;
}

result
AttachmentItem::SetCustomBounds(const Tizen::Graphics::Rectangle& rect) {
	this->SetBounds(rect);


	__pButton->SetBounds(Rectangle(rect.width - 44 - 5, 5, 44, 44));
	this->RequestRedraw();

	return E_SUCCESS;
}

result
AttachmentItem::OnDraw(void) {
	result r = E_SUCCESS;

	Canvas* pCanvas = GetCanvasN();

	if (pCanvas != null)
	{
		Rectangle rect = GetBounds();

		Color color;

		if (__ready) {
			color = Color(38, 55, 75, 255);
		} else {
			color = Color(30, 42, 42, 255);
		}

		pCanvas->FillRoundRectangle(
				color,
				Rectangle(0, 0, rect.width, rect.height),
				Dimension(7,7));

		if (__ready && __pImage) {

			int size = 0;

			if (this->__pImage->GetWidth() > this->__pImage->GetHeight()) {
				size = this->__pImage->GetHeight();
			} else {
				size = this->__pImage->GetWidth();
			}

			pCanvas->DrawBitmap(
							Rectangle(0, 0, rect.width, rect.height),
							*this->__pImage,
							Rectangle(0, 0, size, size)

			);
		} else {
			pCanvas->DrawBitmap(
							Rectangle(
									rect.width/2 - iconSize/2,
									iconTopOffset,
									iconSize,
									iconSize),
							*this->__pIcon);
		}

		if (!this->__ready && this->__pTitleLabel) {
			Point drawPoint;

			drawPoint = Point(10, iconTopOffset + iconSize);
			pCanvas->DrawText(drawPoint, *this->__pTitleLabel);
		}

		if (!this->__ready) {

			int originalWidth = (rect.width - 10 *2);
			int bottomOffset = 15;

			pCanvas->FillRoundRectangle(
					Color(0,0,0,255),
					Rectangle(10, rect.height - bottomOffset, originalWidth, 10),
					Dimension(7,7));

			if (__progress != 0 && __progress <= 100) {

				int width = (int)((originalWidth * __progress)/100);

				pCanvas->FillRoundRectangle(
							Color(86, 156, 219, 255),
							FloatRectangle(10, rect.height - bottomOffset, width, 10),
							FloatDimension(7,7));
			}
		}
	}

	delete pCanvas;
	return r;
}

void
AttachmentItem::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == deleteButton) {
		if (this->__pItemEventListener) {
			this->__pItemEventListener->DeleteButtonPressedAtIndex(this->__index);
		}
	}
}

void
AttachmentItem::SetReady(bool state) {
	this->__ready = state;
	this->Invalidate(true);
}

void
AttachmentItem::UpdateProgressValue(int progress) {
	this->__progress = progress;
	this->Invalidate(true);
}

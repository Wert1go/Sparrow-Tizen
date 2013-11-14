/*
 * UiDialogCustomItem.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include "UiDialogCustomItem.h"

#include <FUi.h>
#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>
#include "ImageCache.h"
#include "ImageView.h"
#include "Util.h"

using namespace Tizen::App;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

UiDialogCustomItem::UiDialogCustomItem() {
	result r;
	Image image;
	r = image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_list.png";
	__pPlaceholder = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	Image imageActive;
		r = imageActive.Construct();
		filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_list_active.png";
		PlaceholderActive = imageActive.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	__pRefreshListener = null;
	__pDialogIcon = null;


}


UiDialogCustomItem::~UiDialogCustomItem() {
	AppLog("UiDialogCustomItem::~UiDialogCustomItem");
	ImageCache::getInstance().CancelLoadingForTarget(this);
	delete __pPlaceholder;
	delete __pDialogIcon;
//	delete __pUrl;
	AppLog("UiDialogCustomItem::~UiDialogCustomItem::Complite");
}

void
UiDialogCustomItem::SetDialog(MDialog *dialog) {
	this->__pDialog = dialog;
}

MDialog *
UiDialogCustomItem::GetDialog() {
	return this->__pDialog;
}

void
UiDialogCustomItem::Init() {
	result r = E_SUCCESS;

	__pImageView = new (std::nothrow) ImageView();
	__pImageView->__pBitmapImage = __pPlaceholder;
	this->AddElement(Rectangle(0,0,110,110), ID_USER_AVATAR, *__pImageView);

	this->AddElement(Rectangle(0,0,110,110), ID_USER_AVATAR_MASK, *__pPlaceholder, PlaceholderActive, PlaceholderActive);

	Color *normalColor = new Color(0,0,0,0);
	Color *selectedColor = new Color(33,63,99,255);
	this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, *normalColor);
	this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_PRESSED, *selectedColor);
	this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_HIGHLIGHTED, *selectedColor);

	delete normalColor;
	delete selectedColor;

	EnrichedText* pUserName = null;
	TextElement* pUsetNameText = null;

	pUserName = new EnrichedText();
	r = pUserName->Construct(Dimension(500, 60));

	pUserName->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pUserName->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pUserName->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
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
	int width, height;
	pUserName->GetSize(width, height);

	AppLogDebug("%d :: %d", width, height);

	Rectangle nameRect = Rectangle(130, 10, width, 60);

	this->AddElement(nameRect, 1111, *pUserName);

	EnrichedText* pDialogLabel = null;
	TextElement* pDialogText = null;

	pDialogLabel = new EnrichedText();
	r = pDialogLabel->Construct(Dimension(500, 60));

	pDialogLabel->SetHorizontalAlignment(TEXT_ALIGNMENT_LEFT);
	pDialogLabel->SetVerticalAlignment(TEXT_ALIGNMENT_BOTTOM);
	pDialogLabel->SetTextWrapStyle(TEXT_WRAP_CHARACTER_WRAP);
	pDialogLabel->SetTextAbbreviationEnabled(true);

	pDialogText = new TextElement();
	if (!this->__pDialog->GetText()) {
		AppLogDebug("ALARM");
	}

	String *text = this->__pDialog->GetText();

	if (text->GetLength() == 0) {
		text = new String(L" ");
	}

	AppLogDebug("pre text %S", this->__pDialog->GetTitle()->GetPointer());
	r = pDialogText->Construct(text->GetPointer());
	AppLogDebug("pre text %S", this->__pDialog->GetTitle()->GetPointer());
	GetErrorMessage(r);
	pDialogText->SetTextColor(Color::GetColor(COLOR_ID_GREY));
	{
		Font font;
		font.Construct(FONT_STYLE_BOLD, 24);
		pDialogText->SetFont(font);
	}
	pDialogLabel->Add(*pDialogText);

	pDialogLabel->GetSize(width, height);

	AppLogDebug("%d :: %d", width, height);
	int xOffset = 130;

	if (this->GetDialog()->GetOut() == 1) {
		xOffset = 160;
	}

	this->AddElement(Rectangle(xOffset, 80, width, 60), 3242, *pDialogLabel);

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

	pTimeLabel->GetSize(width, height);

	this->AddElement(Rectangle(400, 10, 180, 60), 4567, *pTimeLabel);

	//

	this->SetImageUrl(this->__pDialog->GetPhoto());
}

void
UiDialogCustomItem::SetImageUrl(String *url) {
	__pUrl = url;
	ImageCache::getInstance().LoadImageForTarget(__pUrl, this);
}

void
UiDialogCustomItem::SetIndex(int index) {
	__index = index;
}

int
UiDialogCustomItem::GetIndex() {
	return __index;
}

void
UiDialogCustomItem::AddRefreshListener(IRefreshableListView *pRefreshListener) {
	__pRefreshListener = pRefreshListener;
}

void
UiDialogCustomItem::OnImageLoadedN(Bitmap *result) {

	__pDialogIcon = result;

	this->__pImageView->__pBitmapImage = __pDialogIcon;

	__pRefreshListener->RequestUpdateForIndex(__index, ID_USER_AVATAR);
}

void
UiDialogCustomItem::OnErrorN(Error *error) {

}

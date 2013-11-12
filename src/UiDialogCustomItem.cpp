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
UiDialogCustomItem::Init() {

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

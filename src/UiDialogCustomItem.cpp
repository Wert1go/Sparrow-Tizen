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
#include "UiImageView.h"
#include "Util.h"
#include "UiDialogListItem.h"

#include "Resources.h"

using namespace Tizen::App;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

UiDialogCustomItem::UiDialogCustomItem() {
	result r;

	__pPlaceholder = Resources::getInstance().GetNormalRoundImageForm();
	PlaceholderActive = Resources::getInstance().GetSelectedRoundImageForm();

	__pRefreshListener = null;
	__pDialogIcon = null;
	__pDialogListItem = null;
}


UiDialogCustomItem::~UiDialogCustomItem() {
	AppLog("UiDialogCustomItem::~UiDialogCustomItem");
	ImageCache::getInstance().CancelLoadingForTarget(this);
	__pPlaceholder = null;
	PlaceholderActive = null;
	delete __pDialogIcon;
	delete __pDialogListItem;
//	delete __pUrl;
	AppLog("UiDialogCustomItem::~UiDialogCustomItem::Complite");
}

void
UiDialogCustomItem::SetDimension(Dimension *pDimension) {
	this->__pDimension = pDimension;
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

	Color *normalColor = new Color(0,0,0,0);
	Color *normalUnreadColor = new Color(25,31,37,255);
	Color *selectedColor = new Color(33,63,99,255);

	if (this->__pDialog->GetReadState() == 0 && this->__pDialog->GetOut() == 0) {
		this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, *normalUnreadColor);
	} else {
		this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, *normalColor);
	}

	this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_PRESSED, *selectedColor);
	this->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_HIGHLIGHTED, *selectedColor);
	delete normalColor;
	delete normalUnreadColor;
	delete selectedColor;

	Rectangle rect = Rectangle(0,0,__pDimension->width, __pDimension->height);

	__pImageView = new (std::nothrow) UiImageView();
	__pImageView->__pBitmapImage = __pPlaceholder;

	this->AddElement(rect, ID_USER_AVATAR, *__pImageView);


	__pDialogListItem = new UiDialogListItem();
	__pDialogListItem->SetDialog(this->__pDialog);

	this->AddElement(rect, 23, *__pDialogListItem);
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

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
#include "UiCustomItemBackground.h"
#include "UiUserListItem.h"
#include "MUser.h"

#include "Resources.h"

using namespace Tizen::App;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

UiDialogCustomItem::UiDialogCustomItem() {
	result r;

	__index = 0;
	__section = -1;

	__pPlaceholder = Resources::getInstance().GetNormalRoundImageForm();
	PlaceholderActive = Resources::getInstance().GetSelectedRoundImageForm();
	__pDialog = null;
	__pUser = null;
	__pRefreshListener = null;
	__pDialogIcon = null;
	__pDialogListItem = null;
	__pUserListItem = null;
	__pDialogBG = null;
	__pRefreshListener = null;
}


UiDialogCustomItem::~UiDialogCustomItem() {
	AppLog("UiDialogCustomItem::~UiDialogCustomItem");
	ImageCache::getInstance().CancelLoadingForTarget(this);
	__pPlaceholder = null;
	PlaceholderActive = null;
	delete __pDialogIcon;
	delete __pDialogListItem;
//	delete __pUrl;
//	AppLog("UiDialogCustomItem::~UiDialogCustomItem::Complite");
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

	Rectangle rect = Rectangle(0,0,__pDimension->width, __pDimension->height);

	this->__pDialogBG = new UiCustomItemBackground();
	this->__pDialogBG->SetDialog(this->__pDialog);

	this->AddElement(rect, 45, *__pDialogBG);

	__pImageView = new (std::nothrow) UiImageView();
	__pImageView->__pBitmapImage = __pPlaceholder;

	this->AddElement(rect, ID_USER_AVATAR, *__pImageView);

	if (this->__pDialog) {
		__pDialogListItem = new UiDialogListItem();
		__pDialogListItem->SetDialog(this->__pDialog);

		this->AddElement(rect, 23, *__pDialogListItem);
		this->SetImageUrl(this->__pDialog->GetPhoto());
	} else {

		__pUserListItem = new UiUserListItem();
		__pUserListItem->SetUser(this->__pUser);

		this->AddElement(rect, 23, *__pUserListItem);

		if (this->__pUser->GetPhoto()) {
			AppLogDebug("test!!! %S", this->__pUser->GetPhoto()->GetPointer());
			this->SetImageUrl(this->__pUser->GetPhoto());
		}
	}
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
UiDialogCustomItem::SetSection(int section) {
	__section = section;
}
int
UiDialogCustomItem::GetSection() {
	return __section;
}

void
UiDialogCustomItem::AddRefreshListener(IRefreshableListView *pRefreshListener) {
	__pRefreshListener = pRefreshListener;
}

void
UiDialogCustomItem::OnImageLoadedN(Bitmap *result) {
	AppLogDebug("OnImageLoadedN!!!");
	__pDialogIcon = result;

	this->__pImageView->__pBitmapImage = __pDialogIcon;
	if (__pRefreshListener) {
		if (__section != -1) {
			__pRefreshListener->RequestImageUpdateForIndex(__index, __section, ID_USER_AVATAR);
		} else {
			__pRefreshListener->RequestUpdateForIndex(__index, ID_USER_AVATAR);
		}


	}
}

void
UiDialogCustomItem::OnErrorN(Error *error) {

}

void
UiDialogCustomItem::SetUser(MUser *pUser) {
	__pUser = pUser;
}

MUser *
UiDialogCustomItem::GetUser() {
	return __pUser;
}

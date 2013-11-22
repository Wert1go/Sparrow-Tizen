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
	__pImageViews = new LinkedList(SingleObjectDeleter);
}


UiDialogCustomItem::~UiDialogCustomItem() {
	AppLog("UiDialogCustomItem::~UiDialogCustomItem");
	ImageCache::getInstance().CancelLoadingForTarget(this);
	__pPlaceholder = null;
	PlaceholderActive = null;

	delete __pDialogListItem;
	delete __pImageViews;
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
	int size = 100;

	Rectangle rect = Rectangle(0,0,__pDimension->width, __pDimension->height);

	this->__pDialogBG = new UiCustomItemBackground();
	this->__pDialogBG->SetDialog(this->__pDialog);

	this->AddElement(rect, 45, *__pDialogBG);



	if (this->__pDialog) {
		__pDialogListItem = new UiDialogListItem();
		__pDialogListItem->SetDialog(this->__pDialog);

		if (this->__pDialog->GetUsers() && this->__pDialog->GetUsers()->GetCount() > 0) {

			int userCount = this->__pDialog->GetUsers()->GetCount();

			for (int index = 0; index < userCount; index ++) {
				UiImageView *pImageView = new (std::nothrow) UiImageView();

				MUser *user = static_cast<MUser *>(this->__pDialog->GetUsers()->GetAt(index));

				Rectangle imageRect = Rectangle(0,0,0,0);

				float itemSize = 55;
				float centerX = 80;
				float centerY = rect.height/2;
				if (userCount == 4) {

					switch(index) {
					case 0:
						imageRect = Rectangle(centerX - itemSize, centerY - itemSize, itemSize, itemSize);
						break;
					case 1:
						imageRect = Rectangle(centerX, centerY - itemSize, itemSize, itemSize);
						break;
					case 2:
						imageRect = Rectangle(centerX - itemSize, centerY, itemSize, itemSize);
						break;
					case 3:
						imageRect = Rectangle(centerX, centerY, itemSize, itemSize);
						break;
					default:
						break;
					}


				} else if (userCount == 3) {
					switch(index) {
					case 0:
						imageRect = Rectangle(centerX - itemSize, centerY - itemSize, itemSize, itemSize);
						break;
					case 1:
						imageRect = Rectangle(centerX, centerY - itemSize, itemSize, itemSize);
						break;
					case 2:
						imageRect = Rectangle(centerX - itemSize/2, centerY, itemSize, itemSize);
						break;
					default:
						break;
					}

				} else if (userCount == 2) {
					switch(index) {
					case 0:
						imageRect = Rectangle(centerX - itemSize, centerY - itemSize/2, itemSize, itemSize);
						break;
					case 1:
						imageRect = Rectangle(centerX, centerY - itemSize/2, itemSize, itemSize);
						break;

					default:
						break;
					}
				}

				this->AddElement(imageRect, index, *pImageView);
				this->__pImageViews->Add(pImageView);
				this->SetImageUrl(user->GetPhoto(), index);
			}
		} else {
			UiImageView *pImageView = new (std::nothrow) UiImageView();
			pImageView->__pBitmapImage = __pPlaceholder;
			this->AddElement(Rectangle(80 - size/2, rect.height/2 - size/2, size, size), 0, *pImageView);
			this->__pImageViews->Add(pImageView);
			this->SetImageUrl(this->__pDialog->GetPhoto(), 0);
		}

		this->AddElement(rect, 23, *__pDialogListItem);

	} else {

		UiImageView *pImageView = new (std::nothrow) UiImageView();
		pImageView->__pBitmapImage = __pPlaceholder;
		this->AddElement(Rectangle(80 - size/2, rect.height/2 - size/2, size, size), 0, *pImageView);
		this->__pImageViews->Add(pImageView);

		__pUserListItem = new UiUserListItem();
		__pUserListItem->SetUser(this->__pUser);

		this->AddElement(rect, 23, *__pUserListItem);

		if (this->__pUser->GetPhoto()) {
			AppLogDebug("test!!! %S", this->__pUser->GetPhoto()->GetPointer());
			this->SetImageUrl(this->__pUser->GetPhoto(), 0);
		}
	}
}

void
UiDialogCustomItem::SetImageUrl(String *url, int code) {
	__pUrl = url;
	ImageCache::getInstance().LoadImageForTarget(__pUrl, this, new Integer(code));
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
UiDialogCustomItem::OnImageLoadedN(Bitmap *result, Integer *code) {
	AppLogDebug("OnImageLoadedN!!!");

	int index = code->ToInt();

	if (index < this->__pImageViews->GetCount()) {

		UiImageView *imageView = static_cast<UiImageView *>(this->__pImageViews->GetAt(index));

		imageView->__pBitmapImage = result;

		if (__pRefreshListener) {
			if (__section != -1) {
				__pRefreshListener->RequestImageUpdateForIndex(__index, __section, index);
			} else {
				__pRefreshListener->RequestUpdateForIndex(__index, index);
			}
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

/*
 * UiChatCustomItem.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#include "UiChatCustomItem.h"
#include "UiChatListItem.h"
#include "MMessage.h"
#include "IRefreshableListView.h"
#include "UiImageView.h"
#include "ImageCache.h"

using namespace Tizen::Ui;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

UiChatCustomItem::UiChatCustomItem() {
	__pChatListItem = new UiChatListItem();
	__pImageViews = new LinkedList();
	__pRefreshListener = null;
	__pUrtToIndexMap = new HashMapT<String *, Integer *>();
	__pUrtToIndexMap->Construct(100, 0.75);
	__section = -1;
}

UiChatCustomItem::~UiChatCustomItem() {
	delete __pChatListItem;
	__pChatListItem = null;
	ImageCache::getInstance().CancelLoadingForTarget(this);
}

void
UiChatCustomItem::Init() {
	Rectangle rect = Rectangle(0,0,__pDimension->width, __pDimension->height);
	__pChatListItem->SetBubbleDimension(__pBubbleDimension);
	__pChatListItem->SetMessage(this->GetMessage());
	__pChatListItem->SetDialog(this->GetDialog());
	__pChatListItem->SetDrawer(this);
	this->AddElement(rect, 23, *__pChatListItem);
}

void
UiChatCustomItem::SetDimension(Dimension *pDimension) {
	this->__pDimension = pDimension;
}

void
UiChatCustomItem::SetBubbleDimension(Dimension pDimension) {
	__pBubbleDimension = pDimension;
}

void
UiChatCustomItem::SetMessage(MMessage *pMessage) {
	__pMessage = pMessage;
}

MMessage *
UiChatCustomItem::GetMessage() {
	return __pMessage;
}

void
UiChatCustomItem::SetDialog(MDialog *pDialog) {
	__pDialog = pDialog;
}

MDialog *
UiChatCustomItem::GetDialog() {
	return __pDialog;
}


void
UiChatCustomItem::DrawImageFromUrlInRect(String *imageUrl, Rectangle rect) {
	bool exist = false;
	this->__pUrtToIndexMap->ContainsKey(imageUrl, exist);
	if (exist) {
		return;
	}

	AppAssert(imageUrl);

	int index = this->__pImageViews->GetCount();

	UiImageView *pImageView = new (std::nothrow) UiImageView();

	__pUrtToIndexMap->Add(imageUrl, new Integer(index));
	__pImageViews->Add(pImageView);

	this->AddElement(rect, index, *pImageView);
	this->__pImageViews->Add(pImageView);
	this->SetImageUrl(imageUrl, index);

}

void
UiChatCustomItem::AddRefreshListener(IRefreshableListView *RefreshListener) {
	this->__pRefreshListener = RefreshListener;
}


void
UiChatCustomItem::OnImageLoadedN(Bitmap *result, Integer *code) {
	int index = code->ToInt();

	if (index < this->__pImageViews->GetCount() && __pRefreshListener) {

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
UiChatCustomItem::OnErrorN(Error *error) {

}

void
UiChatCustomItem::SetImageUrl(String *url, int code) {
	ImageCache::getInstance().LoadImageForTarget(url, this, new Integer(code));
}

void
UiChatCustomItem::SetIndex(int index) {
	this->__index = index;
}

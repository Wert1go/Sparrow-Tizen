/*
 * UiChatCustomItem.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#include "UiChatCustomItem.h"
#include "UiChatListItem.h"
#include "MMessage.h"

using namespace Tizen::Ui;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

UiChatCustomItem::UiChatCustomItem() {
	__pChatListItem = new UiChatListItem();
}

UiChatCustomItem::~UiChatCustomItem() {
	delete __pChatListItem;
	__pChatListItem = null;
}

void
UiChatCustomItem::Init() {
	Rectangle rect = Rectangle(0,0,__pDimension->width, __pDimension->height);
	__pChatListItem->SetBubbleDimension(__pBubbleDimension);
	__pChatListItem->SetMessage(this->GetMessage());
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

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

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;


UiAttachmentView::UiAttachmentView() {
	__pBitmapImage = null;
	__pAttachment = null;
}

UiAttachmentView::~UiAttachmentView() {

	if (__pBitmapImage) {
		delete __pBitmapImage;
	}
}

bool
UiAttachmentView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	}

//	AppLog("OnDraw::END");
	return true;
}

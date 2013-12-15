/*
 * ImageView.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: developer
 */

#include "UiImageView.h"

#include <FUi.h>
#include <FGraphics.h>
#include "Resources.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;


UiImageView::UiImageView() {
	__pBitmapImage = null;
	__id = 0;
}

UiImageView::~UiImageView() {
	if (__pBitmapImage) {
		delete __pBitmapImage;
	}
}

bool
UiImageView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	} else {
		if (__id == 1) {
			r = canvas.DrawBitmap(rect, *Resources::getInstance().GetNoUserAvatar());
		} else if (__id == 2) {
			r = canvas.DrawBitmap(rect, *Resources::getInstance().GetNoGroupAvatar());
		}
	}

	return true;
}



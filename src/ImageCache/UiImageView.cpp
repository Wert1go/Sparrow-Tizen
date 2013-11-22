/*
 * ImageView.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: developer
 */

#include "UiImageView.h"

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;


UiImageView::UiImageView() {
	__pBitmapImage = null;
}

UiImageView::~UiImageView() {
	delete __pBitmapImage;
}

bool
UiImageView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	}

	return true;
}



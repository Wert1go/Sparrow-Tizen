/*
 * ImageView.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: developer
 */

#include "ImageView.h"

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

ImageView::ImageView() {
	// TODO Auto-generated constructor stub

}

ImageView::~ImageView() {
	// TODO Auto-generated destructor stub
}

bool
ImageView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(rect, *__pBitmapImage);
	}

	return true;
}



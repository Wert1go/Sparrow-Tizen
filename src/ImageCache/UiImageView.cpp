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

//static int borderSize = 108;
static int size = 100;

UiImageView::UiImageView() {
	// TODO Auto-generated constructor stub

}

UiImageView::~UiImageView() {
	// TODO Auto-generated destructor stub
}

bool
UiImageView::OnDraw(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {
	result r;

	if (__pBitmapImage != null)
	{
		r = canvas.DrawBitmap(Rectangle(80 - size/2, rect.height/2 - size/2, size, size), *__pBitmapImage);
	}

	return true;
}



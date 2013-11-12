/*
 * ImageView.h
 *
 *  Created on: Nov 12, 2013
 *      Author: developer
 */

#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class ImageView
: public ICustomElement {
public:
	ImageView();
	virtual ~ImageView();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

public:
	Bitmap *__pBitmapImage;

};

#endif /* IMAGEVIEW_H_ */

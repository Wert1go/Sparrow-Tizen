/*
 * ImageView.h
 *
 *  Created on: Nov 12, 2013
 *      Author: developer
 */

#ifndef UIIMAGEVIEW_H_
#define UIIMAGEVIEW_H_

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class UiImageView
 : public ICustomElement
 , public Object{
public:
	UiImageView();
	virtual ~UiImageView();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

public:
	Bitmap *__pBitmapImage;
	int __id; //0 - nothing /1 - user /2 - chat

};

#endif /* UIIMAGEVIEW_H_ */

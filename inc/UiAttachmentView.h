/*
 * UiAttachmentView.h
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#ifndef UIATTACHMENTVIEW_H_
#define UIATTACHMENTVIEW_H_

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class MAttachment;

class UiAttachmentView
 : public Tizen::Ui::Controls::ICustomElement
 , public Object
 {
public:
	UiAttachmentView();
	virtual ~UiAttachmentView();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

public:
	Bitmap *__pBitmapImage;
	MAttachment *__pAttachment;

};

#endif /* UIATTACHMENTVIEW_H_ */

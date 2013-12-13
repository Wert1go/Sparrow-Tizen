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
class IImageDrawer;

class UiAttachmentView
 : public Tizen::Ui::Controls::ICustomElement
 , public Object
 {
public:
	UiAttachmentView();
	virtual ~UiAttachmentView();

	void SetAttachment(MAttachment *pAttachment);

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

public:
	Bitmap *__pBitmapImage;
	MAttachment *__pAttachment;

	EnrichedText* __pTitleLabel;
	TextElement* __pTitleText;
	Point __titleDrawPoint;

	EnrichedText* __pDurationLabel;
	TextElement* __pDurationText;
	Point __durationDrawPoint;

	EnrichedText* __pTextLabel;
	TextElement* __pTextText;
	Point __textDrawPoint;

	bool __playing;

	IImageDrawer *__pImageDrawer;

};

#endif /* UIATTACHMENTVIEW_H_ */

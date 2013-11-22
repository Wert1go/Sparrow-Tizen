/*
 * UiDialogListItem.h
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#ifndef UIDIALOGLISTITEM_H_
#define UIDIALOGLISTITEM_H_

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class MDialog;

class UiDialogListItem
: public ICustomElement
  {
public:
	UiDialogListItem();
	virtual ~UiDialogListItem();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

	void SetDialog(MDialog *pDialog);
	MDialog *GetDialog();
	void DrawRounders(Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Bitmap *bitmap);

private:
	MDialog *__pDialog;

};

#endif /* UIDIALOGLISTITEM_H_ */

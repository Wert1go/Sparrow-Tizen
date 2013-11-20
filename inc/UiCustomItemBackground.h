/*
 * UiCustomItemBackground.h
 *
 *  Created on: Nov 19, 2013
 *      Author: developer
 */

#ifndef UICUSTOMITEMBACKGROUND_H_
#define UICUSTOMITEMBACKGROUND_H_

#include <FUi.h>
#include <FGraphics.h>

class MDialog;

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class UiCustomItemBackground
 : public ICustomElement
{
public:
	UiCustomItemBackground();
	virtual ~UiCustomItemBackground();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

	void SetDialog(MDialog *pDialog);
	MDialog *GetDialog();
private:
	MDialog *__pDialog;
};

#endif /* UICUSTOMITEMBACKGROUND_H_ */

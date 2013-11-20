/*
 * UiCustomItemBackground.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: developer
 */

#include "UiCustomItemBackground.h"
#include <FUi.h>
#include <FGraphics.h>
#include "MDialog.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

UiCustomItemBackground::UiCustomItemBackground() {
	this->__pDialog = null;
}

UiCustomItemBackground::~UiCustomItemBackground() {
	// TODO Auto-generated destructor stub
}


void
UiCustomItemBackground::SetDialog(MDialog *pDialog) {
	this->__pDialog = pDialog;
}

MDialog *
UiCustomItemBackground::GetDialog() {
	return __pDialog;
}

bool
UiCustomItemBackground::OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status) {

	if (status == LIST_ITEM_DRAWING_STATUS_NORMAL) {
		if (this->__pDialog != null && this->__pDialog->GetReadState() == 0 && this->__pDialog->GetOut() == 0) {
			canvas.FillRectangle(Color(25,31,37,255), rect);
		} else {
			canvas.FillRectangle(Color(0,0,0,0), rect);
		}
	} else {
		canvas.FillRectangle(Color(33,63,99,255), rect);
	}

	return true;
}

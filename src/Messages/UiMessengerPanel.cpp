/*
 * UiMessengerPanel.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#include "UiMessengerPanel.h"

#include <FUiCtrlPanel.h>
#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

UiMessengerPanel::UiMessengerPanel() {
	// TODO Auto-generated constructor stub

}

UiMessengerPanel::~UiMessengerPanel() {
	// TODO Auto-generated destructor stub
}

bool
UiMessengerPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));

	__pEditArea = new ExpandableEditArea();

	__pEditArea->Construct(
			Rectangle(0, 0, 10, 10),
			EXPANDABLE_EDIT_AREA_STYLE_NORMAL,
			EXPANDABLE_EDIT_AREA_TITLE_STYLE_NONE,
			3
	);

	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_NORMAL, Color(115, 120, 145, 255));
	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_LINK, Color(115, 120, 145, 255));

	__pEditArea->SetGuideTextColor(Color(115, 120, 145, 255));

	__pEditArea->SetTextSize(40);
	__pEditArea->SetColor(EDIT_STATUS_NORMAL, Color(0,0,0,255));
	__pEditArea->SetColor(EDIT_STATUS_HIGHLIGHTED, Color(0,0,0,255));
	__pEditArea->SetColor(EDIT_STATUS_PRESSED, Color(0,0,0,255));
	__pEditArea->SetGuideText(L"Введите сообщение");
	AddControl(__pEditArea);

	__pSendButton = new (std::nothrow) Button();
	__pSendButton->Construct(Rectangle(120, 20, 100, 100));

	__pSendButton->SetActionId(45);
	//pAuthButton->AddActionEventListener(*this);

	Image image;
	image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/keyboard_icon_send.png";
	Bitmap *imageN = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	__pSendButton->SetColor(BUTTON_STATUS_NORMAL, Color(44, 51, 59, 255));
	__pSendButton->SetColor(BUTTON_STATUS_PRESSED,  Color(255, 255, 255, 255));

	__pSendButton->SetNormalBitmap(FloatPoint(90/2 - 63/2, 66/2 - 63/2), *imageN);
	this->SetBackgroundColor(Color(0,0,0,255));
	delete imageN;

	AddControl(__pSendButton);

	return true;
}

result
UiMessengerPanel::OnInitializing(void)
{
	result r = E_SUCCESS;

	AppLog("test");





	return r;
}

result
UiMessengerPanel::OnTerminating() {
	result r = E_SUCCESS;
	return r;
}

ExpandableEditArea*
UiMessengerPanel::GetEditArea() {
	return __pEditArea;
}

Button*
UiMessengerPanel::GetSendButton() {
	return __pSendButton;
}

Button*
UiMessengerPanel::GetAttachButton() {
	return __pAttachButton;
}

void
UiMessengerPanel::SetRectangle(FloatRectangle rect) {
	this->SetBounds(rect);
	float offset = 16;

	FloatRectangle editBounds = FloatRectangle(
			0 + offset,
			0 + offset,
			rect.width - offset*2 - 90 - offset,
			rect.height - offset*2
	);

	this->__pEditArea->SetBounds(editBounds);

	this->__pSendButton->SetBounds(FloatRectangle(editBounds.x + editBounds.width + offset, rect.height/2 - 66/2, 90, 66));
	this->Invalidate(true);
}

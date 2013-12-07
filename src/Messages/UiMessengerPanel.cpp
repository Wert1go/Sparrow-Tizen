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
#include "IRequestAttachmentDelete.h"
#include "AttachmentItem.h"
#include "MAttachment.h"

static int itemSize = 170;
static int itemOffset = 20;

const int editAreaOffset = 10;

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

UiMessengerPanel::UiMessengerPanel() :
		__bButtonMoved(false),
		__pScrollPanel(null) {

	__pItems = new LinkedList();
	__pAttachmentOwner = null;
}

UiMessengerPanel::~UiMessengerPanel() {
	// TODO Auto-generated destructor stub
}

bool
UiMessengerPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));

	int count = 0;

	__pScrollPanel = new Panel();
	__pScrollPanel->Construct(Rectangle(0, 0, itemSize * count + itemOffset*(count + 1), 0));
	__pScrollPanel->AddTouchEventListener(*this);

	AddControl(*__pScrollPanel);

//	for (int i = 0; i < count; i++) {
//		AttachmentItem *pItem = this->CreateItem(i);
//		__pScrollPanel->AddControl(*pItem);
//		__pItems->Add(pItem);
//	}

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

	Image image;
	image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/keyboard_icon_send.png";
	Bitmap *imageN = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	__pSendButton->SetColor(BUTTON_STATUS_NORMAL, Color(44, 51, 59, 255));
	__pSendButton->SetColor(BUTTON_STATUS_PRESSED,  Color(255, 255, 255, 255));
	__pSendButton->SetNormalBitmap(FloatPoint(90/2 - 63/2, 66/2 - 63/2), *imageN);


	__pAttachButton= new (std::nothrow) Button();
	__pAttachButton->Construct(Rectangle(120, 20, 100, 100));

	__pAttachButton->SetActionId(46);

	__pAttachButton->SetColor(BUTTON_STATUS_NORMAL, Color(44, 51, 59, 255));
	__pAttachButton->SetColor(BUTTON_STATUS_PRESSED,  Color(255, 255, 255, 255));

	Image attachImage;
	attachImage.Construct();
	filepath = App::GetInstance()->GetAppResourcePath() + L"Images/keyboard_icon_attach.png";
	Bitmap *attachImageN = attachImage.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	__pAttachButton->SetNormalBitmap(FloatPoint(90/2 - 63/2, 66/2 - 63/2), *attachImageN);

	this->SetBackgroundColor(Color(23, 30, 38,255));

	delete imageN;
	delete attachImageN;

	AddControl(__pSendButton);
	AddControl(__pAttachButton);

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
	float offset = editAreaOffset;

	if (this->GetCount() > 0) {
		this->UpdateScrollPanel();
	}

	float yOffset = 0 + this->__pScrollPanel->GetBounds().y + this->__pScrollPanel->GetBounds().height;

	float restHeight = rect.height - yOffset;

	AppLog("test: %f", restHeight);

	FloatRectangle editBounds = FloatRectangle(
			0 + offset*2 + 90,
			yOffset + offset,
			rect.width - offset*3 - 90 * 2 - offset,
			restHeight - offset*2
	);

	this->__pEditArea->SetBounds(editBounds);

	this->__pSendButton->SetBounds(FloatRectangle(
			editBounds.x + editBounds.width + offset,
			(rect.height  - 100/2) - 66/2,
			90,
			66));

	this->__pAttachButton->SetBounds(FloatRectangle(
			offset,
			(rect.height  - 100/2) - 66/2,
			90,
			66));

	this->Invalidate(true);
}

/************************ SCROLL PANEL ***************************/

void
UiMessengerPanel::OnTouchPressed(const Control& source, const Point& currentPosition, const TouchEventInfo & touchInfo)
{
	__PointLastTouch.x = currentPosition.x;
	__bButtonMoved = false;
}

void
UiMessengerPanel::OnTouchMoved(const Control& source, const Point& currentPosition, const TouchEventInfo& touchInfo)
{
	int x, y;

	__pScrollPanel->GetPosition(x, y);
	x = x + (currentPosition.x - __PointLastTouch.x);

	int limit = __pScrollPanel->GetSize().width - this->GetBounds().width;

	if (x > 0 && x < 50) {
		x = 0;
	}

	if (x > -(limit) && x < -(limit - 50)) {
		x = -(limit);
	}

	if(x <= 0 && x >= -(limit))
	{
		__pScrollPanel->SetPosition(x, y);
		__pScrollPanel->RequestRedraw();
	}
	__bButtonMoved = true;
}

/************************ ITEMS **************************/

int
UiMessengerPanel::GetCount() {
	return __pItems->GetCount();
}

void
UiMessengerPanel::ReloadData() {

//	this->__pScrollPanel->RemoveAllControls();

	for (int i = 0; i < this->GetCount(); i++) {
		AttachmentItem *pItem = static_cast<AttachmentItem *>(this->__pItems->GetAt(i));
		pItem->SetCustomBounds(Rectangle((itemOffset * (i + 1)) + itemSize * i, 0, itemSize, itemSize));
		pItem->__index = i;

	}

	this->UpdateScrollPanel();
}

AttachmentItem *
UiMessengerPanel::CreateItem(int index, MAttachment *attach) {
	AttachmentItem *pItem;

	pItem = new AttachmentItem();
	pItem->__pAttachment = attach;
	pItem->__index = index;
	pItem->Initialize();
//	pItem->SetCustomBounds(Rectangle((itemOffset * (index + 1)) + itemSize * index, 0, itemSize, itemSize));
	pItem->AddTouchEventListener(*this);
	pItem->__pItemEventListener = this;
	return pItem;
}

void
UiMessengerPanel::DeleteButtonPressedAtIndex(int index) {
	AttachmentItem *pItem = static_cast<AttachmentItem *>(this->__pItems->GetAt(index));
	this->__pAttachmentOwner->RequestAttachmentDelete(pItem->__pAttachment, index);
}

void
UiMessengerPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId){
	if (actionId < 10) {

//		AttachmentItem *pItem = this->CreateItem(this->GetCount());
//		__pScrollPanel->AddControl(*pItem);
//		__pItems->Add(pItem);
//
//		this->UpdateScrollPanel();
	}
}

void
UiMessengerPanel::UpdateScrollPanel() {
	AppLog("UiMessengerPanel");
	int containerWidth = this->GetBounds().width;

	int xOffset = 0;
	int targetWidth = itemSize * this->GetCount() + itemOffset*(this->GetCount() + 1);

	if (targetWidth > containerWidth) {
		xOffset = -(targetWidth - containerWidth);
	}

	int count = this->GetCount();
	int yOffset = count > 0 ? 10 : 0;
	int height = count > 0 ? itemSize : 0;

	this->__pScrollPanel->SetBounds(Rectangle(
				xOffset,
				yOffset,
				targetWidth,
				height)
				);
}

void
UiMessengerPanel::AddAttachment(MAttachment *attachment) {
	int i = this->GetCount();
	AttachmentItem *pItem = this->CreateItem(i, attachment);

	this->__pItems->Add(pItem);
	this->__pScrollPanel->AddControl(pItem);

	this->UpdateScrollPanel();
}

void
UiMessengerPanel::RemoveAttachment(MAttachment *attachment, int index) {
	AttachmentItem *pItem = static_cast<AttachmentItem *>(this->__pItems->GetAt(index));
	pItem->__pItemEventListener = null;
	this->__pItems->Remove(*pItem);
	this->__pScrollPanel->RemoveControl(*pItem);
	this->ReloadData();
}

void
UiMessengerPanel::SetAttachmentReady(bool state, int index, MAttachment *attachment) {
	if (index >= this->__pItems->GetCount()) {
		return;
	}

	AttachmentItem *pItem = static_cast<AttachmentItem *>(this->__pItems->GetAt(index));
	pItem->SetReady(state);
}

void
UiMessengerPanel::UpdateAttachmentProgress(int progress, int index, MAttachment *attachment) {
	if (index >= this->__pItems->GetCount()) {
		return;
	}

	AttachmentItem *pItem = static_cast<AttachmentItem *>(this->__pItems->GetAt(index));
	pItem->UpdateProgressValue(progress);
}

void
UiMessengerPanel::ClearAttachments() {
	this->__pItems->RemoveAll();
	this->__pScrollPanel->RemoveAllControls();
	this->UpdateScrollPanel();
}

void
UiMessengerPanel::AddAttachments(LinkedList *pAttachments) {
	for (int i = 0; i < pAttachments->GetCount(); i++) {
		MAttachment *attach = static_cast<MAttachment *>(pAttachments->GetAt(i));

		AttachmentItem *pItem = this->CreateItem(this->GetCount(), attach);

		this->__pItems->Add(pItem);
		this->__pScrollPanel->AddControl(pItem);
	}

	this->ReloadData();
}


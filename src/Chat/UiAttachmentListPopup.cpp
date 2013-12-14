/*
 * UiAttachmentListPopup.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#include "UiAttachmentListPopup.h"
#include "Resources.h"
#include "IPopupHandler.h"
#include <FApp.h>
#include "AppResourceId.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::App;

UiAttachmentListPopup::UiAttachmentListPopup() {
	__pListView = null;
	this->__pPopupHandler = null;

	pPhotoIcon = null;
	pAudioIcon = null;
	pVideoIcon = null;
	pDocIcon = null;
	pMapIcon = null;
}

UiAttachmentListPopup::~UiAttachmentListPopup() {
	delete pPhotoIcon;
	delete pAudioIcon;
	delete pVideoIcon;
	delete pDocIcon;
	delete pMapIcon;
}

result
UiAttachmentListPopup::OnInitializing(void) {
	result r = E_SUCCESS;

	SetPropagatedKeyEventListener(this);
	String titleString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH, titleString);
	this->SetTitleText(titleString);

	this->SetColor(Color(52, 87, 119, 255));
	this->SetTitleTextColor(Color(0, 0, 0, 255));

	Rectangle clientRect;
	clientRect = this->GetClientAreaBounds();

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8,8, 255));
	Color *separatorColor = new (std::nothrow) Color(48, 48, 48, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Test1");
	__pItemContext->AddElement(2, L"Test2");

	return r;
}

result
UiAttachmentListPopup::OnTerminating(void) {
	result r = E_SUCCESS;

	__pPopupHandler = null;

//	if (__pItemContext) {
//		delete __pItemContext;
//	}

	return r;
}

void
UiAttachmentListPopup::ShowPopup(void) {
	SetShowState(true);
	Show();
}

void
UiAttachmentListPopup::HidePopup(void) {
	SetShowState(false);
	Invalidate(true);
}

bool
UiAttachmentListPopup::OnKeyPressed(Control& source, const KeyEventInfo& keyEventInfo) {
	return false;
}
bool
UiAttachmentListPopup::OnKeyReleased(Control& source, const KeyEventInfo& keyEventInfo) {
	if ((keyEventInfo.GetKeyCode() == KEY_ESC ||keyEventInfo.GetKeyCode() == KEY_BACK) && source.GetShowState() == true)
	{
		SetShowState(false);
	}
	return false;
}

bool
UiAttachmentListPopup::OnPreviewKeyPressed(Control& source, const KeyEventInfo& keyEventInfo) {
	return false;
}

bool
UiAttachmentListPopup::OnPreviewKeyReleased(Control& source, const KeyEventInfo& keyEventInfo) {
	return false;
}

bool
UiAttachmentListPopup::TranslateKeyEventInfo(Control& source, KeyEventInfo& keyEventInfo) {
	return false;
}

//****************************** LIST VIEW **************************/

void
UiAttachmentListPopup::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state) {

}


void
UiAttachmentListPopup::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status) {
	AppLog("sfdfd");
	if (status == LIST_ITEM_STATUS_SELECTED && __pPopupHandler) {
		AppLog("sfdfd111111111111");
		__pPopupHandler->DidSelectItemInPopup(index, 0);
	}
}

void
UiAttachmentListPopup::OnListViewItemSwept(ListView &listView, int index, SweepDirection direction)
{

}


ListItemBase*
UiAttachmentListPopup::CreateItem(int index, int itemWidth)
{
	CustomItem* pItem = new CustomItem();
	ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

	int height = (this->GetBounds().height - 80)/this->GetItemCount();

	pItem->Construct(Dimension(itemWidth, height), style);
	pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Color(31, 52, 71, 255));
	pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_PRESSED, Color(33, 63, 99, 255));
	pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_HIGHLIGHTED, Color(33, 63, 99, 255));

	Bitmap *pIcon = null;
	String text;

	if (index == 0) {
		Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH_PHOTO, text);
		pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_photo.png");
		this->pPhotoIcon = pIcon;
	} else if (index == 1) {
		Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH_VIDEO, text);
		pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_video.png");
		this->pVideoIcon = pIcon;
	} else if (index == 2) {
		Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH_AUDIO, text);
		pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_audio.png");
		this->pAudioIcon = pIcon;
	} else if (index == 3) {
		Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH_DOC, text);
		pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_document.png");
		this->pDocIcon = pIcon;
	} else if (index == 4) {
		Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH_MAP, text);
		pIcon = Resources::getInstance().LoadBitmapNamed(L"icon_location.png");
		this->pMapIcon = pIcon;
	}

	pItem->AddElement(
			Rectangle(10, height/2 - 72/2, 72, 72),
			1,
			*pIcon,
			pIcon,
			pIcon);

	pItem->AddElement(
			Rectangle(102, 0, itemWidth - 102, height),
			2,
			text,
			42,
			Color(255, 255, 255, 255),
			Color(255, 255, 255, 255),
			Color(255, 255, 255, 255) );

    return pItem;
}

bool
UiAttachmentListPopup::DeleteItem(int index, ListItemBase* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

int
UiAttachmentListPopup::GetItemCount(void)
{
	return 5;
}

result
UiAttachmentListPopup::OnDraw(void) {

	Popup::OnDraw();
	String titleString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_ATTACH, titleString);
	this->SetTitleText(titleString);
	if (this->__pListView) {
		this->__pListView->UpdateList();
	}
	return E_SUCCESS;
}

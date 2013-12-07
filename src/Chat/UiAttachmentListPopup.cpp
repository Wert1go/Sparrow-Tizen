/*
 * UiAttachmentListPopup.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#include "UiAttachmentListPopup.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

UiAttachmentListPopup::UiAttachmentListPopup() {
	__pListView = null;
}

UiAttachmentListPopup::~UiAttachmentListPopup() {
	// TODO Auto-generated destructor stub
}

result
UiAttachmentListPopup::OnInitializing(void) {
	result r = E_SUCCESS;

	SetTitleText(L"Вложить");
	SetPropagatedKeyEventListener(this);

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

}
void
UiAttachmentListPopup::OnListViewItemSwept(ListView &listView, int index, SweepDirection direction)
{

}


ListItemBase*
UiAttachmentListPopup::CreateItem(int index, int itemWidth)
{
	AppLogDebug("AppLogDebug(OnInitializing);");
	CustomItem* pItem = new CustomItem();
	ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

	int height = this->GetBounds().height/this->GetItemCount();

	pItem->Construct(Dimension(itemWidth, height), style);
	pItem->SetContextItem(__pItemContext);

	pItem->AddElement(Rectangle(0,0, itemWidth, height), index, L"TEST", false);

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

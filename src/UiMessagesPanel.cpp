/*
 * UiMessagesPanel.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include <FApp.h>

#include "UiMessagesPanel.h"
#include "UiDialogCustomItem.h"
#include "UpdateUnit.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiMessagesPanel::UiMessagesPanel() {
	// TODO Auto-generated constructor stub

}

UiMessagesPanel::~UiMessagesPanel() {
	// TODO Auto-generated destructor stub
}

bool
UiMessagesPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));	// Should be set proper area at OnInitializing().
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));

	return true;
}

result
UiMessagesPanel::OnInitializing(void)
{

	result r = E_SUCCESS;
	Rectangle clientRect;

	// Resize
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	AppAssert(pForm);
	clientRect = pForm->GetClientAreaBounds();
	SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height));

	// Creates an instance of ListView
	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);

	Color *separatorColor = new (std::nothrow) Color(28, 28, 28, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	// Adds the list view to the form
	AddControl(__pListView);

	// Creates an instance of ListContextItem
	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(ID_CONTEXT_ITEM_1, L"Test1");
	__pItemContext->AddElement(ID_CONTEXT_ITEM_2, L"Test2");

	return r;
}

result
UiMessagesPanel::OnTerminating() {
	result r = E_SUCCESS;
	return r;
}

// IListViewItemEventListener implementation
void
UiMessagesPanel::OnListViewItemStateChanged(ListView &listView, int index, int elementId, ListItemStatus status)
{

}

void
UiMessagesPanel::OnListViewContextItemStateChanged(ListView &listView, int index, int elementId, ListContextItemStatus state)
{
    switch (elementId)
    {
    case ID_CONTEXT_ITEM_1:
        {
            // ....
        }
        break;
    case ID_CONTEXT_ITEM_2:
        {
            // ....
        }
        break;
    default:
        break;
    }
}

void
UiMessagesPanel::OnListViewItemSwept(ListView &listView, int index, SweepDirection direction)
{
    // ....
}

// IListViewItemProvider implementation
ListItemBase*
UiMessagesPanel::CreateItem(int index, int itemWidth)
{
	AppLog("CreateItem :: %d", index);
    // Creates an instance of CustomItem
	UiDialogCustomItem *pItem = new UiDialogCustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

    int height = 110;

    pItem->Construct(Dimension(itemWidth, height), style);
    pItem->SetContextItem(__pItemContext);
    pItem->Init();

    pItem->SetIndex(index);
    pItem->AddRefreshListener(this);
    String *url = null;
    if (index%2 == 0) {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07819.png");
    } else if (index%3 == 0) {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07818.jpg");
    } else if (index%4 == 0) {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07810.jpg");
    } else if (index%5 ==0) {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07809.jpg");
    } else if(index%6 == 0) {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07803.jpg");
    } else {
    	url = new String(L"http://media.oboobs.ru/boobs_preview/07801.jpg");
    }

    pItem->SetImageUrl(url);

    return pItem;
}

bool
UiMessagesPanel::DeleteItem(int index, ListItemBase* pItem, int itemWidth)
{
	AppLog("DeleteItem");

    delete pItem;
    pItem = null;
    return true;
}

int
UiMessagesPanel::GetItemCount(void)
{
    return 300;
}

void
UiMessagesPanel::RequestUpdateForIndex(int index, int elementId) {
	ArrayList *list = new ArrayList();
	list->Construct(1);

	UpdateUnit *updateUnit = new UpdateUnit();

	updateUnit->__index = index;
	updateUnit->__requestId = elementId;

	list->Add(updateUnit);
	this->SendUserEvent(111111, list);
	Tizen::App::App::GetInstance()->SendUserEvent(1, 0);
}

void
UiMessagesPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {

	if (requestId == 111111 && pArgs->GetCount() > 0) {
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));

		__pListView->RefreshList(unit->__index, unit->__requestId);
	}

	delete pArgs;
}

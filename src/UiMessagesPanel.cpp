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
#include "MDialogDao.h"

#include "RDialogResponse.h"
#include "MDialogsDescriptor.h"
#include "Error.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "AuthManager.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiMessagesPanel::UiMessagesPanel() {
	// TODO Auto-generated constructor stub
	__pDialogsList = null;
	__pDialogRequestOperation = null;
}

UiMessagesPanel::~UiMessagesPanel() {
	if (__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
	}

	//ImageCache::getInstance().CancelLoadingForTarget(this);
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

	this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
	SendRequest();
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

    int height = 136;

    pItem->Construct(Dimension(itemWidth, height), style);
    pItem->SetContextItem(__pItemContext);
    pItem->SetDimension(new Dimension(itemWidth, height));
    pItem->SetIndex(index);
    pItem->AddRefreshListener(this);

    MDialog *dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));
    pItem->SetDialog(dialog);

    pItem->Init();

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
	if (this->GetDialogsList()) {
		return this->GetDialogsList()->GetCount();
	} else {
		return 0;
	}
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
	Tizen::App::App::GetInstance()->SendUserEvent(111111, 0);
	AppLog("ping %d", index);
}

void
UiMessagesPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {

	AppLog("pong %d", -1);
	if (requestId == 111111 && pArgs->GetCount() > 0) {
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));
		AppLog("pong %d", unit->__index);
		__pListView->RefreshList(unit->__index, unit->__requestId);
	} else if (requestId == 222222) {
		__pListView->UpdateList();
	}

	delete pArgs;
}

// ************************** DATA ***************************//

void
UiMessagesPanel::SetDialogsList(LinkedList *list) {

	if (this->__pDialogsList) {
		delete this->__pDialogsList;
	}
	AppLog("SetDialogs");
	__pDialogsList = list;
}

LinkedList *
UiMessagesPanel::GetDialogsList() {
	return __pDialogsList;
}

void
UiMessagesPanel::OnSuccessN(RestResponse *result) {
	if(__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}

	RDialogResponse *response = static_cast<RDialogResponse *>(result);

	this->SetDialogsList(response->GetDialogs());
	this->SendUserEvent(222222, 0);
	Tizen::App::App::GetInstance()->SendUserEvent(222222, 0);
}

void
UiMessagesPanel::OnErrorN(Error *error) {
	if(__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}
}

void
UiMessagesPanel::SendRequest() {

	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"code"), new String(L""
		"var a = API.messages.getDialogs({\"offset\" : 0, \"count\":20});"
		"var l = a.items@.user_id;"
		"var c = a.items@.chat_id;"
		"var i = 0;"
		"var uids = [];"
		"var j;"
		"while (i < c.length) {"
		"	i=i+1;"
		"	j = API.messages.getChatUsers({\"chat_id\" : c[i]});"
		"	if (j != false) {"
		"		uids = uids + [j];"
		"		if (parseInt(c[i]) != 0)"
		"		l = l + j;"
		"	}"
		"};"
		"var b = API.users.get({\"user_ids\": l, \"fields\": \"photo_100,photo_50,online\"});"
		"return {\"chat_uids\" : uids, \"users\": b, \"messages\": a};"
	));

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pDialogRequestOperation) {
		__pDialogRequestOperation = new RestRequestOperation(GET_DIALOGS_EXECUTE, new String(L"execute"), params);
		__pDialogRequestOperation->AddEventListener(this);
		__pDialogRequestOperation->SetResponseDescriptor(new MDialogsDescriptor());
		RestClient::getInstance().PerformOperation(__pDialogRequestOperation);
	}
}

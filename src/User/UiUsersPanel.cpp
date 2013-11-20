/*
 * UiUsersPanel.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: developer
 */

#include <FBase.h>

#include "UiUsersPanel.h"
#include "AuthManager.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "UserDescriptor.h"
#include "UserRestResponse.h"
#include "UpdateUnit.h"

#include "UiDialogCustomItem.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiUsersPanel::UiUsersPanel() {
	__pUserRequestOperation = null;
	this->__pUsersList = null;
}

UiUsersPanel::~UiUsersPanel() {
	if (__pUserRequestOperation) {
		__pUserRequestOperation->AddEventListener(null);
		__pUserRequestOperation = null;
	}

}

bool
UiUsersPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));	// Should be set proper area at OnInitializing().
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	return true;
}

result
UiUsersPanel::OnInitializing(void) {

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"UiUsersPanel");
	// Resize
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	AppAssert(pForm);
	clientRect = pForm->GetClientAreaBounds();
	SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height));

	// Creates an instance of ListView
	__pListView = new GroupedListView();
	__pListView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height), GROUPED_LIST_VIEW_STYLE_INDEXED, true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddGroupedListViewItemEventListener(*this);

	Color *separatorColor = new (std::nothrow) Color(28, 28, 28, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	// Adds the list view to the form
	AddControl(__pListView);

	// Creates an instance of ListContextItem
	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Test1");
	__pItemContext->AddElement(2, L"Test2");

	return r;
}

result
UiUsersPanel::OnTerminating(void) {
	return E_SUCCESS;
}

void
UiUsersPanel::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	RequestUsers();
}

void
UiUsersPanel::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}


/********************* GROUPED LIST VIEW ********************/

void
UiUsersPanel::OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
{
    switch (elementId)
    {
    case 1:
        {
            // ....
        }
        break;

        break;
    default:
        break;
    }
}

void
UiUsersPanel::OnGroupedListViewContextItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListContextItemStatus state)
{
    switch (elementId)
    {
    case 1:
        {
            // ....
        }
        break;
    default:
        break;
    }
}

// IGroupedListViewItemEventListener
void
UiUsersPanel::OnGroupedListViewItemSwept(GroupedListView &listView, int groupIndex,  int itemIndex, SweepDirection direction)
{
    // ....
}

int
UiUsersPanel::GetGroupCount(void)
{
    return 1;
}

int
UiUsersPanel::GetItemCount(int groupIndex)
{
    int itemCount = 0;
//    switch (groupIndex)
//    {
//    case 0:
//        {
//            itemCount = 7;
//        }
//        break;
//    case 1:
//        {
//            itemCount = 5;
//        }
//        break;
//    case 2:
//        {
//            itemCount = 3;
//        }
//        break;
//    default:
//        break;
//    }

	if (this->__pUsersList) {
		return __pUsersList->GetCount();
	}

    return itemCount;
}

// IGroupedListViewItemProvider
GroupItem*
UiUsersPanel::CreateGroupItem(int groupIndex, int itemWidth)
{
    String text("Group ");
    text.Append(groupIndex+1);
    GroupItem* pItem = new GroupItem();
    pItem->Construct(Dimension(itemWidth, 40));
    pItem->SetElement(text, null);

    return pItem;
}

ListItemBase*
UiUsersPanel::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{

	AppLogDebug("%d :: %d", itemIndex, groupIndex);
    // Creates an instance of CustomItem
	UiDialogCustomItem* pItem = new UiDialogCustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;


    int height = 136;

    pItem->Construct(Dimension(itemWidth, height), style);
	pItem->SetContextItem(__pItemContext);
	pItem->SetDimension(new Dimension(itemWidth, height));
	pItem->SetIndex(itemIndex);
	pItem->SetSection(groupIndex);
	AppLogDebug("111%d :: %d", itemIndex, groupIndex);
	pItem->AddRefreshListener(this);
	AppLogDebug("2222%d :: %d", itemIndex, groupIndex);
	MUser *dialog = static_cast<MUser *>(this->__pUsersList->GetAt(itemIndex));

	AppLog("%S", dialog->GetMiniPhoto()->GetPointer());

	pItem->SetUser(dialog);

	pItem->Init();

    return pItem;
}

bool
UiUsersPanel::DeleteItem(int groupIndex, int itemIndex, ListItemBase* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

bool
UiUsersPanel::DeleteGroupItem(int groupIndex, GroupItem* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

void
UiUsersPanel::RequestUpdateForIndex(int index, int elementId) {
	ArrayList *list = new ArrayList();
	list->Construct(1);

	UpdateUnit *updateUnit = new UpdateUnit();

	updateUnit->__index = index;
	updateUnit->__requestId = elementId;

	list->Add(updateUnit);

	this->SendUserEvent(111111, list);
	Tizen::App::App::GetInstance()->SendUserEvent(111111, 0);
}

void
UiUsersPanel::RequestUsers() {
	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"fields"), new String(L"photo_50,photo_100,last_seen,online"));
	params->Add(new String(L"hints"), new String(L"1"));
	//params->Add(new String(L"count"), new String(L"100"));
	AppLogDebug("test");
	if (!__pUserRequestOperation) {
		__pUserRequestOperation = new RestRequestOperation(GET_FRIENDS, new String(L"friends.get"), params);
		__pUserRequestOperation->AddEventListener(this);
		__pUserRequestOperation->SetResponseDescriptor(new UserDescriptor());
		RestClient::getInstance().PerformOperation(__pUserRequestOperation);
	}
}

void
UiUsersPanel::OnSuccessN(RestResponse *response) {

	if (response->GetOperationCode() == GET_FRIENDS) {

		if(__pUserRequestOperation) {
			__pUserRequestOperation->AddEventListener(null);
			__pUserRequestOperation = null;
		}

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);
		this->__pUsersList = userResponse->GetUsers();
	}

	this->SendUserEvent(response->GetOperationCode(), 0);
	Tizen::App::App::GetInstance()->SendUserEvent(response->GetOperationCode(), 0);
}

void
UiUsersPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {



	if (requestId == 111111) {
		AppLog("OnUserEventReceivedN!!!!!11");
		AppAssert(pArgs->GetCount() > 0);
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));

		AppLogDebug("%d :: %d", unit->__index, unit->__requestId);


		__pListView->RefreshList(0, unit->__index, unit->__requestId);
	} else {

		try {
			this->__pListView->UpdateList();
		} catch (...) {

		}
	}

	delete pArgs;
}

void
UiUsersPanel::OnErrorN(Error *error) {
	if (__pUserRequestOperation) {
		__pUserRequestOperation->AddEventListener(null);
		__pUserRequestOperation = null;
	}

	delete error;
}


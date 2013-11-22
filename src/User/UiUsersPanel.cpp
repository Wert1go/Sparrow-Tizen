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
#include "RFriendsDescriptor.h"
#include "UserRestResponse.h"
#include "UpdateUnit.h"
#include "MUser.h"
#include "MUserDao.h"
#include "UsersComparer.h"
#include "CustomGroupItem.h"
#include "SceneRegister.h"

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

	__pSearchBar = new SearchBar();
	__pSearchBar->Construct(Rectangle(0, 0, clientRect.width, 100));
	__pSearchBar->SetText(L"Поиск");
	__pSearchBar->AddSearchBarEventListener(*this);
	__pSearchBar->AddTextEventListener(*this);
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_NORMAL, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_DISABLED, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_NORMAL, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_DISABLED, Color(115, 120, 145, 255));
	__pSearchBar->SetColor(Color(23, 30, 38, 255));

	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_NORMAL, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_DISABLED, Color(115, 120, 145, 255));

	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_NORMAL, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_HIGHLIGHTED, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(0, 0, 0, 255));

	AddControl(__pSearchBar);

	__pListView = new GroupedListView();
	__pListView->Construct(Rectangle(0, 110, clientRect.width, clientRect.height - 100 - 110), GROUPED_LIST_VIEW_STYLE_INDEXED, true, true);
	__pListView->SetItemProvider(*this);
	__pListView->AddGroupedListViewItemEventListener(*this);


	__pListView->SetFastScrollIndex(L"#0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЭЮЯ", true);
	__pListView->AddFastScrollListener(*this);
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
UiUsersPanel::OnTerminating(void) {
	return E_SUCCESS;
}

void
UiUsersPanel::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	this->__pUsersList = MUserDao::getInstance().GetFriendsN();

	if (this->__pListView) {
		this->__pListView->UpdateList();
	}

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
	if (state != LIST_ITEM_STATUS_SELECTED) {
		return;
	}

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ArrayList *paramsList = new (std::nothrow) ArrayList();
	paramsList->Construct();

	LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));

	MUser *user = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

	paramsList->Add(new Integer(user->GetUid()));
	pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
}

void
UiUsersPanel::OnGroupedListViewContextItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListContextItemStatus state)
{

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
    return __pSectionItemsList->GetCount();
}

int
UiUsersPanel::GetItemCount(int groupIndex)
{
    LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));
    return sectionItems->GetCount();
}

// IGroupedListViewItemProvider
GroupItem*
UiUsersPanel::CreateGroupItem(int groupIndex, int itemWidth)
{
    String text("");

    String *title = static_cast<String *>(this->__pSectionTitlesList->GetAt(groupIndex));
    text.Append(title->GetPointer());
    text.ToUpperCase();
    CustomGroupItem* pItem = new CustomGroupItem();
    pItem->Construct(Dimension(itemWidth, 40));
    pItem->SetElement(text, null);
    pItem->SetTextColor(Color(60, 110, 156, 255));
    pItem->SetTextSize(32);

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

	pItem->AddRefreshListener(this);
	LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));
	MUser *dialog = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

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
UiUsersPanel::RequestImageUpdateForIndex(int index, int section, int elementId) {
	ArrayList *list = new ArrayList();
	list->Construct(1);

	UpdateUnit *updateUnit = new UpdateUnit();

	updateUnit->__index = index;
	updateUnit->__section = section;
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
	params->Add(new String(L"order"), new String(L"hints"));

	if (!__pUserRequestOperation) {
		__pUserRequestOperation = new RestRequestOperation(GET_FRIENDS, new String(L"friends.get"), params);
		__pUserRequestOperation->AddEventListener(this);
		__pUserRequestOperation->SetResponseDescriptor(new RFriendsDescriptor());
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

		AppAssert(pArgs->GetCount() > 0);
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));

		__pListView->RefreshList(unit->__section, unit->__index, unit->__requestId);
	} else {
		SplitUsersToSections();
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

void
UiUsersPanel::OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index) {
	AppLog("OnFastScrollIndexSelected: %S", index.GetPointer());

	for (int i = 0; i < this->__pSectionTitlesList->GetCount(); i ++) {
		String *scrollIndex = static_cast<String *>(__pSectionTitlesList->GetAt(i));

		if (scrollIndex->Equals(index.GetPointer(), false)) {
			this->__pListView->ScrollToItem(i, 0);
			break;
		}
	}
}

void
UiUsersPanel::SplitUsersToSections() {
	AppAssert(__pUsersList);
	__pSectionItemsList->RemoveAll();
	__pSectionTitlesList->RemoveAll();

	__pSectionTitlesList->Add(new String(L"Важные"));

	int importantCount = 5;

	if (__pUsersList->GetCount() >= importantCount) {
		LinkedList *users = new LinkedList();

		for (int index = 0; index < importantCount; index ++) {
			users->Add(__pUsersList->GetAt(index));
		}
		__pSectionItemsList->Add(users);

		__pUsersList->RemoveItems(0, 5);
	}
	AppLog("SORT");
	UsersComparer *comparer = new UsersComparer();
	__pUsersList->Sort(*comparer);

	String letters = L"#0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЭЮЯ";
	String fastScrollIndex("");

	AppLog("BEGIN");

	for (int index = 0; index < letters.GetLength(); index ++) {
		wchar_t ltr;
		letters.GetCharAt(index, ltr);
		String letter = String(ltr);

		bool begin = false;
		int startIndex = -1;
		int endIndex = -1;

		LinkedList *listToAdd;

		for(int userIndex = 0; userIndex < __pUsersList->GetCount(); userIndex++) {

			MUser *user = static_cast<MUser *>(__pUsersList->GetAt(userIndex));
			//AppLog("userIndex %d %S", userIndex, user->GetFirstName()->GetPointer());
			wchar_t ltr;
			user->GetFirstName()->GetCharAt(0, ltr);
			String firstLetter(ltr);

			if (letter.Equals(firstLetter, false)) {
				//AppLog("compare:: %S == %S", firstLetter.GetPointer(), user->GetFirstName()->GetPointer());

				if (begin) {
					AppAssert(listToAdd);
					listToAdd->Add(user);
				} else {
					begin = true;
					LinkedList *users = new LinkedList();
					users->Add(user);
					listToAdd = users;
					__pSectionItemsList->Add(users);
					__pSectionTitlesList->Add(new String(letter));
					startIndex = userIndex;
					fastScrollIndex.Append(letter);
				}
			} else {
				if (begin) {
					listToAdd = null;
					endIndex = userIndex;
					break;
				}
			}

		}

		if (startIndex != -1 && endIndex != -1) {
			//AppLog("%d :: %d", startIndex, endIndex);
			__pUsersList->RemoveItems(startIndex, endIndex - startIndex + 1);
		}
	}

	AppLog("END");

	__pListView->SetFastScrollIndex(fastScrollIndex, true);

	delete comparer;
}

void
UiUsersPanel::OnTextValueChanged(const Tizen::Ui::Control& source) {

	String string = this->__pSearchBar->GetText();

	if (string.GetLength() == 0) {
		AppLog("OnTextValueChanged!!!!!");
		this->__pUsersList = MUserDao::getInstance().GetFriendsN();
	} else {
		AppLog("OnTextValueChanged %S", string.GetPointer());

		this->__pUsersList = MUserDao::getInstance().SearchUsers(new String(string.GetPointer()));

		AppLog("RESULTS %d", this->__pUsersList->GetCount());
	}

	SplitUsersToSections();
	this->__pListView->UpdateList();
}

void
UiUsersPanel::RequestUpdateForIndex(int index, int elementId) {

}

void
UiUsersPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {

}

void
UiUsersPanel::SetCurrentDisplayMode(int mode) {
	if (mode == 2) {
		//do nothing
	} else if (mode == 1) {
		this->__pUsersList = MUserDao::getInstance().GetFriendsN(true);
	} else {
		this->__pUsersList = MUserDao::getInstance().GetFriendsN();
	}

	SplitUsersToSections();
	this->__pListView->UpdateList();
}

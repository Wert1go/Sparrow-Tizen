/*
 * BaseUserController.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#include "BaseUserController.h"

#include "MUser.h"
#include "MUserDao.h"
#include "UsersComparer.h"
#include "CustomGroupItem.h"
#include "UpdateUnit.h"
#include "SceneRegister.h"
#include "UiDialogCustomItem.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;


BaseUserController::BaseUserController() {
	// TODO Auto-generated constructor stub

}

BaseUserController::~BaseUserController() {
	delete __pSectionItemsList;
	delete __pSectionTitlesList;
	delete __pScrollIndex;
}

void
BaseUserController::InitData() {
	__pUsersList = null;
	__pScrollIndex = new String(L"#0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЭЮЯ");
	__pSectionItemsList = new LinkedList();
	__pSectionTitlesList = new LinkedList();
}

/********************* GROUPED LIST VIEW ********************/

void
BaseUserController::OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
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
BaseUserController::OnGroupedListViewContextItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListContextItemStatus state)
{

}

// IGroupedListViewItemEventListener
void
BaseUserController::OnGroupedListViewItemSwept(GroupedListView &listView, int groupIndex,  int itemIndex, SweepDirection direction)
{
    // ....
}

int
BaseUserController::GetGroupCount(void)
{
    return __pSectionItemsList->GetCount();
}

int
BaseUserController::GetItemCount(int groupIndex)
{
    LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));
    return sectionItems->GetCount();
}

// IGroupedListViewItemProvider
GroupItem*
BaseUserController::CreateGroupItem(int groupIndex, int itemWidth)
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
BaseUserController::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
//	AppLogDebug("%d :: %d", itemIndex, groupIndex);
	UiDialogCustomItem* pItem = new UiDialogCustomItem();
    ListAnnexStyle style = this->GetCurrentAnnexStyle();

    int height = 136;

    pItem->Construct(Dimension(itemWidth, height), style);
	pItem->SetContextItem(__pItemContext);

	float annexWidth = pItem->GetAnnexWidth(style);

	pItem->SetDimension(new Dimension(itemWidth - annexWidth, height));
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
BaseUserController::DeleteItem(int groupIndex, int itemIndex, ListItemBase* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

bool
BaseUserController::DeleteGroupItem(int groupIndex, GroupItem* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

void
BaseUserController::SplitUsersToSections() {
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

	this->SplitUsersToSectionsByAlphabet();
}

void
BaseUserController::SplitUsersToSectionsByAlphabet() {
	AppLog("SORT");
	UsersComparer *comparer = new UsersComparer();
	__pUsersList->Sort(*comparer);

	String letters = this->__pScrollIndex->GetPointer();
	String fastScrollIndex("");

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
//			AppLog("userIndex %d %S", userIndex, user->GetFirstName()->GetPointer());
			wchar_t ltr;
			user->GetFirstName()->GetCharAt(0, ltr);
			String firstLetter(ltr);

			if (letter.Equals(firstLetter, false)) {
//				AppLog("compare:: %S == %S", firstLetter.GetPointer(), user->GetFirstName()->GetPointer());

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
					endIndex = userIndex-1;
					break;
				}
			}

		}

		if (startIndex != -1 && endIndex != -1) {
			__pUsersList->RemoveItems(startIndex, endIndex);
		}
	}

	__pListView->SetFastScrollIndex(fastScrollIndex, true);
	AppLog("COMPLITE");
	delete comparer;
}

//IMage update

void
BaseUserController::RequestImageUpdateForIndex(int index, int section, int elementId) {
	ArrayList *list = new ArrayList();
	list->Construct(1);

	UpdateUnit *updateUnit = new UpdateUnit();

	updateUnit->__index = index;
	updateUnit->__section = section;
	updateUnit->__requestId = elementId;

	list->Add(updateUnit);

	this->RequestUiUpdate(111111, list);

//	//TODO virtual!
//	this->SendUserEvent(111111, list);
//	Tizen::App::App::GetInstance()->SendUserEvent(111111, 0);
}

void
BaseUserController::RequestUpdateForIndex(int index, int elementId) {

}

ListAnnexStyle
BaseUserController::GetCurrentAnnexStyle() {
	return LIST_ANNEX_STYLE_NORMAL;
}

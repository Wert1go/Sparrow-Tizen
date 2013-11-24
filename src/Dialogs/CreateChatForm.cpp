/*
 * CreateChatForm.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#include "CreateChatForm.h"

#include <FBase.h>
#include <FApp.h>

#include "MUser.h"
#include "MUserDao.h"
#include "UpdateUnit.h"
#include "Resources.h"
#include "SceneRegister.h"
#include "UiDialogCustomItem.h"
#include "CustomGroupItem.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

CreateChatForm::CreateChatForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);
	this->InitData();
}

CreateChatForm::~CreateChatForm() {
	// TODO Auto-generated destructor stub
}

result
CreateChatForm::OnInitializing(void) {

	Header *pHeader = this->GetHeader();
	pHeader->SetColor(Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(33, 63, 99, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(33, 63, 99, 255));
	pHeader->SetButtonTextColor(BUTTON_ITEM_STATUS_NORMAL, Color(255, 255, 255, 255));
	pHeader->AddActionEventListener(*this);
	pHeader->SetTitleTextColor(Color(255, 255, 255, 255));
	String titleText(L"Новое сообщение");

	pHeader->SetTitleText(titleText);

	String cancelText(L"Отмена");

	ButtonItem *cancelButton = new ButtonItem();
	cancelButton->Construct(BUTTON_ITEM_STYLE_TEXT, 23);
	cancelButton->SetText(cancelText);
	pHeader->SetButton(BUTTON_POSITION_RIGHT, *cancelButton);

	Rectangle clientRect = this->GetClientAreaBounds();

	__pSearchBar = new SearchBar();
	__pSearchBar->Construct(Rectangle(0, 0, clientRect.width, 100));
	__pSearchBar->SetGuideText(L"Поиск");
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
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - 100), GROUPED_LIST_VIEW_STYLE_INDEXED, true, true);
	__pListView->SetItemProvider(*this);
	__pListView->AddGroupedListViewItemEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8,8, 255));

	__pListView->SetFastScrollIndex(this->__pScrollIndex->GetPointer(), true);
	__pListView->AddFastScrollListener(*this);
	Color *separatorColor = new (std::nothrow) Color(48, 48, 48, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Test1");
	__pItemContext->AddElement(2, L"Test2");

	__pItemContextCommon = new ListContextItem();
	__pItemContextCommon->Construct();
	__pItemContextCommon->AddElement(1, L"Test1");
	__pItemContextCommon->AddElement(2, L"Test2");

	return E_SUCCESS;
}

result
CreateChatForm::OnTerminating(void) {
	return E_SUCCESS;
}

void
CreateChatForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	this->__pUsersList = MUserDao::getInstance().GetFriendsN();

	if (this->__pListView) {
		SplitUsersToSections();
		this->__pListView->UpdateList();
	}
}

void
CreateChatForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}


void
CreateChatForm::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
}

void
CreateChatForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

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
CreateChatForm::OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index) {
	for (int i = 0; i < this->__pSectionTitlesList->GetCount(); i ++) {
		String *scrollIndex = static_cast<String *>(__pSectionTitlesList->GetAt(i));

		if (scrollIndex->Equals(index.GetPointer(), false)) {
			this->__pListView->ScrollToItem(i, 0);
			break;
		}
	}
}

void
CreateChatForm::OnTextValueChanged(const Tizen::Ui::Control& source) {

	String string = this->__pSearchBar->GetText();

	if (string.GetLength() == 0) {
		this->__pUsersList = MUserDao::getInstance().GetFriendsN();
	} else {
		this->__pUsersList = MUserDao::getInstance().SearchUsers(new String(string.GetPointer()));
	}

	SplitUsersToSections();
	this->__pListView->UpdateList();
}

void
CreateChatForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}


void
CreateChatForm::OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
{
	if (state != LIST_ITEM_STATUS_SELECTED) {
		return;
	}

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	if (groupIndex == 0) {
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_SELECT_USER, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), 0);
	} else {
		ArrayList *paramsList = new (std::nothrow) ArrayList();
		paramsList->Construct();

		LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex - 1));

		MUser *user = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

		paramsList->Add(new Integer(user->GetUid()));
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
	}
}

int
CreateChatForm::GetGroupCount(void)
{
    return __pSectionItemsList->GetCount() + 1;
}

int
CreateChatForm::GetItemCount(int groupIndex)
{
	if (groupIndex == 0) {
		return 1;
	} else {
		LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex - 1));
		return sectionItems->GetCount();
	}
}

// IGroupedListViewItemProvider
GroupItem*
CreateChatForm::CreateGroupItem(int groupIndex, int itemWidth)
{
    String text("");

    if (groupIndex == 0) {
    	GroupItem* pItem = new GroupItem();
    	pItem->Construct(Dimension(itemWidth, 0));
    	return pItem;
    } else {

		String *title = static_cast<String *>(this->__pSectionTitlesList->GetAt(groupIndex - 1));
		text.Append(title->GetPointer());
		text.ToUpperCase();
		CustomGroupItem* pItem = new CustomGroupItem();
		pItem->Construct(Dimension(itemWidth, 40));
		pItem->SetElement(text, null);
		pItem->SetTextColor(Color(60, 110, 156, 255));
		pItem->SetTextSize(32);

		return pItem;
    }
}

ListItemBase*
CreateChatForm::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	AppLogDebug("%d :: %d", itemIndex, groupIndex);

	if (groupIndex == 0) {
		CustomItem *pItem = new CustomItem();
		ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;
		int height = 150;

		pItem->Construct(Dimension(itemWidth, height), style);
		pItem->SetContextItem(__pItemContextCommon);

		String createChat(L"Создать беседу");

		Bitmap *normalIcon = Resources::getInstance().LoadBitmapNamed(L"group_list_icon_add_normal.png");
		Bitmap *selectedIcon = Resources::getInstance().LoadBitmapNamed(L"group_list_icon_add_press.png");

		float offset = 30;
		Rectangle iconRect = Rectangle(offset, height/2 - 72/2, 72, 72);

		Rectangle textRect = Rectangle(offset + 90, height/2 - 72/2, itemWidth - offset + 90, 72);

		pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Color(8, 8,8, 255));

		pItem->AddElement(iconRect, 0, *normalIcon, selectedIcon, selectedIcon);
		pItem->AddElement(textRect, 1, createChat, 32, Color(255, 255, 255, 255), Color(255, 255, 255, 255),  Color(255, 255, 255, 255));

		return pItem;
	} else {
		UiDialogCustomItem* pItem = new UiDialogCustomItem();
		ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

		int height = 136;

		pItem->Construct(Dimension(itemWidth, height), style);
		pItem->SetContextItem(__pItemContext);
		pItem->SetDimension(new Dimension(itemWidth, height));
		pItem->SetIndex(itemIndex);
		pItem->SetSection(groupIndex);
		pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Color(8, 8,8, 255));
		pItem->AddRefreshListener(this);

		LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex - 1));
		MUser *dialog = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

		pItem->SetUser(dialog);

		pItem->Init();

		return pItem;
	}
}

void
CreateChatForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

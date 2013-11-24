/*
 * SelectChatUsersForm.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#include "SelectChatUsersForm.h"

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

SelectChatUsersForm::SelectChatUsersForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);
	this->InitData();
	__pSelectedUsers = new LinkedList();
}

SelectChatUsersForm::~SelectChatUsersForm() {
	// TODO Auto-generated destructor stub
}

result
SelectChatUsersForm::OnInitializing(void) {

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"SelectChatUsersForm");

	clientRect = this->GetClientAreaBounds();

	Header *pHeader = this->GetHeader();
	pHeader->SetColor(Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(33, 63, 99, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(33, 63, 99, 255));
	pHeader->SetButtonTextColor(BUTTON_ITEM_STATUS_NORMAL, Color(255, 255, 255, 255));
	pHeader->AddActionEventListener(*this);
	pHeader->SetTitleTextColor(Color(255, 255, 255, 255));
	String titleText(L"Беседа");

	pHeader->SetTitleText(titleText);

	String cancelText(L"Далее");

	ButtonItem *cancelButton = new ButtonItem();
	cancelButton->Construct(BUTTON_ITEM_STYLE_TEXT, 23);
	cancelButton->SetText(cancelText);
	pHeader->SetButton(BUTTON_POSITION_RIGHT, *cancelButton);

	__pListView = new GroupedListView();
	__pListView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height), GROUPED_LIST_VIEW_STYLE_INDEXED, true, true);
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

	return r;
}

result
SelectChatUsersForm::OnTerminating(void) {
	return E_SUCCESS;
}

void
SelectChatUsersForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	this->__pUsersList = MUserDao::getInstance().GetFriendsN();

	if (this->__pListView) {
		SplitUsersToSections();
		this->__pListView->UpdateList();
	}
}

void
SelectChatUsersForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
SelectChatUsersForm::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
}

void
SelectChatUsersForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

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

ListItemBase*
SelectChatUsersForm::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	UiDialogCustomItem* pItem = new UiDialogCustomItem();
    ListAnnexStyle style = this->GetCurrentAnnexStyle();

    int height = 136;

    pItem->Construct(Dimension(itemWidth - 63, height), style);
	pItem->SetContextItem(__pItemContext);

	pItem->SetDimension(new Dimension(itemWidth - 63, height));
	pItem->SetIndex(itemIndex);
	pItem->SetSection(groupIndex);

	pItem->AddRefreshListener(this);

	LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));
	MUser *dialog = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

	pItem->SetUser(dialog);

	pItem->Init();

    return pItem;
}

void
SelectChatUsersForm::OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index) {

	for (int i = 0; i < this->__pSectionTitlesList->GetCount(); i ++) {
		String *scrollIndex = static_cast<String *>(__pSectionTitlesList->GetAt(i));

		if (scrollIndex->Equals(index.GetPointer(), false)) {
			this->__pListView->ScrollToItem(i, 0);
			break;
		}
	}
}

void
SelectChatUsersForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {

	if (__pSelectedUsers && __pSelectedUsers->GetCount() > 0) {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_CREATE_CONV, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), __pSelectedUsers);
	}
}

void
SelectChatUsersForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
SelectChatUsersForm::OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state)
{
	AppLog("OnGroupedListViewItemStateChanged");

	LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));

	MUser *user = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

	if (__pSelectedUsers->Contains(*user)) {
		this->__pSelectedUsers->Remove(*user);
	} else {
		__pSelectedUsers->Add(user);
	}
}

ListAnnexStyle
SelectChatUsersForm::GetCurrentAnnexStyle() {
	return LIST_ANNEX_STYLE_MARK;
}

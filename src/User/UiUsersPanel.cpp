/*
 * UiUsersPanel.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: developer
 */

#include <FBase.h>
#include <FApp.h>

#include "UiUsersPanel.h"
#include "AuthManager.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "RFriendsDescriptor.h"
#include "UserRestResponse.h"
#include "UpdateUnit.h"
#include "MUser.h"
#include "MUserDao.h"
#include "BaseUserController.h"

#include "SceneRegister.h"

#include "UiDialogCustomItem.h"
#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiUsersPanel::UiUsersPanel() {
	__pUserRequestOperation = null;
	this->InitData();
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
	clientRect = pForm->GetContentAreaBounds();
	SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height));

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
	__pListView->Construct(Rectangle(0, 110, clientRect.width, clientRect.height - 100 - 110), GROUPED_LIST_VIEW_STYLE_INDEXED, true, true);
	__pListView->SetItemProvider(*this);
	__pListView->AddGroupedListViewItemEventListener(*this);
	__pListView->SetSweepEnabled(false);

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

void
UiUsersPanel::RequestUsers() {
	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"fields"), new String(USER_FILEDS));
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

	this->RequestUiUpdate(response->GetOperationCode(), 0);
}

void
UiUsersPanel::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
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
UiUsersPanel::OnTextValueChanged(const Tizen::Ui::Control& source) {

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

result
UiUsersPanel::OnDraw() {

	String importantString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH, importantString);
	__pSearchBar->SetGuideText(importantString);

	return E_SUCCESS;
}


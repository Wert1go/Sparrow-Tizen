/*
 * SearchUserPanel.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#include "SearchUserPanel.h"

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
#include "AppResourceId.h"
#include "UiDialogCustomItem.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

static String searchTextHolder;

SearchUserPanel::SearchUserPanel() {
	this->InitData();
	__pUsersSuggest = new LinkedList();
	__pUsersRequests = new LinkedList();
	__pUsersSearchResults = new LinkedList();
	__pListUpdateTimer = null;
	__pUserRequestsOperation = null;
	__pUserSuggestOperation = null;
	__pUserSearchOperation = null;
}

SearchUserPanel::~SearchUserPanel() {
	// TODO Auto-generated destructor stub
}

bool
SearchUserPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));	// Should be set proper area at OnInitializing().
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	return true;
}

result
SearchUserPanel::OnInitializing(void) {

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"SearchUserPanel");
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
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - 100), GROUPED_LIST_VIEW_STYLE_INDEXED, true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddGroupedListViewItemEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8, 8, 255));
	__pListView->SetSweepEnabled(false);
	__pListView->SetItemDividerColor(Color(48, 48, 48, 255));

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Test1");
	__pItemContext->AddElement(2, L"Test2");

	return r;
}

result
SearchUserPanel::OnTerminating(void) {
	return E_SUCCESS;
}

void
SearchUserPanel::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	AppLog("OnSceneActivatedN");
	GetUserRequests();
	GetSuggests();
}

void
SearchUserPanel::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
SearchUserPanel::GetUserRequests() {
	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"code"), new String(L""
			"var a = API.friends.getRequests({\"offset\" : 0, \"extended\":0, \"out\":0});"
			"if (a.items.length > 0) {"
				"var b = API.users.get({\"user_ids\": a.items, \"fields\":\"photo_50,photo_100,last_seen,online,is_friend,photo_200\"});"
				"return {\"items\": b};"
			"} else {"
				"return {\"items\": []};"
			"}"
			""));

	if (!__pUserRequestsOperation) {
		__pUserRequestsOperation = new RestRequestOperation(GET_FRIENDS_REQUEST, new String(L"execute"), params);
		__pUserRequestsOperation->AddEventListener(this);

		RFriendsDescriptor *descriptor = new RFriendsDescriptor();
		descriptor->SetPersisterActive(false);
		__pUserRequestsOperation->SetResponseDescriptor(descriptor);
		RestClient::getInstance().PerformOperation(__pUserRequestsOperation);
	}
}

void
SearchUserPanel::SearchUserWithString(String searchText) {
	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"fields"), new String(USER_FILEDS));
	params->Add(new String(L"q"), new String(searchText));
	params->Add(new String(L"count"), new String(L"100"));

	if (!__pUserSearchOperation) {
		__pUserSearchOperation = new RestRequestOperation(SEARCH_USERS, new String(L"users.search"), params);
		__pUserSearchOperation->AddEventListener(this);
		RFriendsDescriptor *descriptor = new RFriendsDescriptor();
		descriptor->SetPersisterActive(false);
		__pUserSearchOperation->SetResponseDescriptor(descriptor);
		RestClient::getInstance().PerformOperation(__pUserSearchOperation);
	}
}

void
SearchUserPanel::GetSuggests() {
	HashMap *params = new HashMap();
	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"code"), new String(L""
				"var a = API.friends.getSuggestions({\"count\" : 20,\"fields\":\"photo_50,photo_100,last_seen,online,is_friend,photo_200\"});"
				"return {\"items\": a};"
				""));

	if (!__pUserSuggestOperation) {
		__pUserSuggestOperation = new RestRequestOperation(GET_FRIENDS_SUGGESST, new String(L"execute"), params);
		__pUserSuggestOperation->AddEventListener(this);
		RFriendsDescriptor *descriptor = new RFriendsDescriptor();
		descriptor->SetPersisterActive(false);
		__pUserSuggestOperation->SetResponseDescriptor(descriptor);
		RestClient::getInstance().PerformOperation(__pUserSuggestOperation);
	}
}

void
SearchUserPanel::OnSuccessN(RestResponse *response) {

	if (response->GetOperationCode() == GET_FRIENDS_SUGGESST) {

		if(__pUserSuggestOperation) {
			__pUserSuggestOperation->AddEventListener(null);
			__pUserSuggestOperation = null;
		}

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);
		this->__pUsersSuggest->RemoveAll();

		for (int i = 0; i < userResponse->GetUsers()->GetCount(); i++) {
			MUser *user = static_cast<MUser *>(userResponse->GetUsers()->GetAt(i));
			user->__isPending = 1;
		}

		this->__pUsersSuggest->AddItems(*userResponse->GetUsers());
	} else if (response->GetOperationCode() == GET_FRIENDS_REQUEST) {

		if(__pUserRequestsOperation) {
			__pUserRequestsOperation->AddEventListener(null);
			__pUserRequestsOperation = null;
		}

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);
		this->__pUsersRequests->RemoveAll();
		this->__pUsersRequests->AddItems(*userResponse->GetUsers());


	} else if (response->GetOperationCode() == SEARCH_USERS) {

		if(__pUserSearchOperation) {
			__pUserSearchOperation->AddEventListener(null);
			__pUserSearchOperation = null;
		}

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);
		this->__pUsersSearchResults->RemoveAll();
		this->__pUsersSearchResults->AddItems(*userResponse->GetUsers());


	}

	this->RequestUiUpdate(response->GetOperationCode(), 0);
}

void
SearchUserPanel::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
}

void
SearchUserPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

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
SearchUserPanel::OnTimerExpired (Timer &timer) {
	if (this->__pListUpdateTimer) {
		if (searchTextHolder != NULL) {
			AppLog("2OnTimerExpired %S", searchTextHolder.GetPointer());
			if (searchTextHolder.GetLength() == 0) {
				this->__pUsersSearchResults->RemoveAll();
				SplitUsersToSections();
			} else {
				AppLog("1OnTimerExpired %S", searchTextHolder.GetPointer());
				this->SearchUserWithString(searchTextHolder);
			}
		}

		delete this->__pListUpdateTimer;
		this->__pListUpdateTimer = null;
	}
}

void
SearchUserPanel::OnTextValueChanged(const Tizen::Ui::Control& source) {

	String string = this->__pSearchBar->GetText();
	searchTextHolder = string;

	AppLog("%S", searchTextHolder.GetPointer());
	if (string.GetLength() == 0) {
		this->__pUsersSearchResults->RemoveAll();
		SplitUsersToSections();
		this->__pListView->UpdateList();
	} else {
		if (this->__pListUpdateTimer) {
			this->__pListUpdateTimer->Cancel();
			delete this->__pListUpdateTimer;
			this->__pListUpdateTimer = null;
		}

		this->__pListUpdateTimer = new Timer();
		this->__pListUpdateTimer->Construct(*this);
		this->__pListUpdateTimer->Start(700);

		//
	}
}

void
SearchUserPanel::OnErrorN(Error *error) {
	if (__pUserSearchOperation) {
		__pUserSearchOperation->AddEventListener(null);
		__pUserSearchOperation = null;
	}

	if (__pUserSuggestOperation) {
		__pUserSuggestOperation->AddEventListener(null);
		__pUserSuggestOperation = null;
	}

	if (__pUserRequestsOperation) {
		__pUserRequestsOperation->AddEventListener(null);
		__pUserRequestsOperation = null;
	}

	delete error;
}

void
SearchUserPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {

}

void
SearchUserPanel::SplitUsersToSections() {

	AppLog("SplitUsersToSections++++++++++");

	__pSectionItemsList->RemoveAll();
	__pSectionTitlesList->RemoveAll();

	if (this->__pUsersSearchResults->GetCount() > 0) {
		__pSectionTitlesList->Add(new String(L"Глобальный поиск"));
		LinkedList *users = new LinkedList();
		users->AddItems(*this->__pUsersSearchResults);
		__pSectionItemsList->Add(users);
	} else {
		if(this->__pUsersRequests->GetCount() > 0) {
			__pSectionTitlesList->Add(new String(L"Заявки в друзья"));
			LinkedList *users = new LinkedList();
			users->AddItems(*this->__pUsersRequests);
			__pSectionItemsList->Add(users);
		}

		if(this->__pUsersSuggest->GetCount() > 0) {
			__pSectionTitlesList->Add(new String(L"Возможные друзья"));
			LinkedList *users = new LinkedList();
			users->AddItems(*this->__pUsersSuggest);
			__pSectionItemsList->Add(users);
		}

	}
}

void
SearchUserPanel::OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state) {
	if (state != LIST_ITEM_STATUS_SELECTED) {
		return;
	}

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ArrayList *paramsList = new (std::nothrow) ArrayList();
	paramsList->Construct();

	LinkedList *sectionItems = static_cast<LinkedList*>(__pSectionItemsList->GetAt(groupIndex));

	MUser *user = static_cast<MUser *>(sectionItems->GetAt(itemIndex));

	paramsList->Add(user);

	pSceneManager->GoForward(ForwardSceneTransition(SCENE_PROFILE, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
}

result
SearchUserPanel::OnDraw() {

	String importantString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH, importantString);
	__pSearchBar->SetGuideText(importantString);

	return E_SUCCESS;
}

/*
 * UiContactsPanel.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: developer
 */

#include "UiContactsPanel.h"

#include "UiDialogCustomItem.h"
#include "AppResourceId.h"
#include "UserDescriptor.h"
#include "UserRestResponse.h"
#include "MUserDao.h"
#include "AuthManager.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "RContactsDescriptor.h"
#include "MUserDao.h"
#include "UsersComparer.h"

#include "SceneRegister.h"

#include <FMedia.h>
#include <FApp.h>
#include <FSocial.h>
#include <FSystem.h>

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Social;

#define ID_BUTTON_IMPORT 			101

UiContactsPanel::UiContactsPanel() {
	this->InitData();
	__pUserImportOperation = null;
	__pContactsList = null;
	__pUsersList = null;
}

UiContactsPanel::~UiContactsPanel() {
	if (__pUsersList) {
		__pUsersList->RemoveAll(true);
		delete __pUsersList;
		__pUsersList = null;
	}
}

bool
UiContactsPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));	// Should be set proper area at OnInitializing().
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	return true;
}

result
UiContactsPanel::OnInitializing(void) {

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"UiContactsPanel");
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

	int buttonWidth = 0;
	int yOffset = 0;

	buttonWidth = (clientRect.width * 84)/100;
	yOffset = (clientRect.height * 48)/100;

	Color *textNormalColor = new (std::nothrow) Color(255, 255, 255, 255);
	Color *textSelectedColor = new (std::nothrow) Color(0, 0, 0, 255);
	Color *buttonNormalColor = new (std::nothrow) Color(52, 87, 119, 255);
	Color *buttonSelectedColor = new (std::nothrow) Color(255, 255, 255, 255);

	pImportButton = new (std::nothrow) Button();
	pImportButton->Construct(Rectangle(clientRect.width/2 - buttonWidth/2, yOffset, buttonWidth, 80));

	pImportButton->SetActionId(ID_BUTTON_IMPORT);
	pImportButton->AddActionEventListener(*this);
	pImportButton->SetTextColor(*textNormalColor);
	pImportButton->SetHighlightedTextColor(*textSelectedColor);
	pImportButton->SetColor(BUTTON_STATUS_NORMAL, *buttonNormalColor);
	pImportButton->SetColor(BUTTON_STATUS_PRESSED, *buttonSelectedColor);

	r = AddControl(pImportButton);

	delete textNormalColor;
	delete textSelectedColor;
	delete buttonNormalColor;
	delete buttonSelectedColor;

	Color *labelTextColor = new (std::nothrow) Color(86, 86, 93, 255);

	yOffset = pImportButton->GetBounds().y + pImportButton->GetBounds().height + 10;
	buttonWidth = (clientRect.width * 82)/100;

	Image image;
	r = image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"empty_bg.png";
	Bitmap *pBackgroundBitmap = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	pInfoLabel = new (std::nothrow) TextBox();
	pInfoLabel->Construct(Rectangle(clientRect.width/2 - buttonWidth/2, yOffset, buttonWidth, 160), TEXT_BOX_BORDER_NONE);

	pInfoLabel->SetTextAlignment(ALIGNMENT_CENTER);
	pInfoLabel->SetTextColor(TEXT_BOX_TEXT_COLOR_NORMAL, *labelTextColor);
	pInfoLabel->SetTextColor(TEXT_BOX_TEXT_COLOR_DISABLED, *labelTextColor);
	pInfoLabel->SetTextSize(28);
	pInfoLabel->SetFocusable(false);
	pInfoLabel->SetEnabled(false);
	pInfoLabel->SetDragEnabled(false);
	pInfoLabel->SetDropEnabled(false);

	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_NORMAL, *pBackgroundBitmap);
	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_HIGHLIGHTED, *pBackgroundBitmap);
	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_DISABLED, *pBackgroundBitmap);

	r = AddControl(pInfoLabel);

	delete pBackgroundBitmap;
	delete labelTextColor;

	return r;
}

result
UiContactsPanel::OnTerminating(void) {
	return E_SUCCESS;
}

void
UiContactsPanel::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	this->__pUsersList = MUserDao::getInstance().GetContactsN();
	this->UpdateScreen();

}

void
UiContactsPanel::UpdateScreen() {
	if (this->__pUsersList->GetCount() == 0) {
		this->pImportButton->SetShowState(true);
		this->pInfoLabel->SetShowState(true);
		this->__pListView->SetShowState(false);
		this->__pSearchBar->SetShowState(false);
	} else {
		this->pImportButton->SetShowState(false);
		this->pInfoLabel->SetShowState(false);
		this->__pListView->SetShowState(true);
		this->__pSearchBar->SetShowState(true);

		if (this->__pListView) {
			SplitUsersToSections();
			this->__pListView->UpdateList();
		}
	}
}

void
UiContactsPanel::SplitUsersToSections() {
	AppAssert(__pUsersList);

	__pSectionItemsList->RemoveAll();
	__pSectionTitlesList->RemoveAll();

	this->SplitUsersToSectionsByAlphabet();
}

void
UiContactsPanel::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

result
UiContactsPanel::OnDraw() {

	String importantString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH, importantString);
	__pSearchBar->SetGuideText(importantString);

	String syncString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SYNC_CONTACTS, syncString);
	pImportButton->SetText(syncString);

	String syncDescrString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SYNC_CONTACTS_DESCR, syncDescrString);
	pInfoLabel->SetText(syncDescrString);

	return E_SUCCESS;
}

void
UiContactsPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == ID_BUTTON_IMPORT) {
		//do import

		ImportContacts();
	}
}

void
UiContactsPanel::ImportContacts() {
	AddressbookManager* pAddressbookMgr = AddressbookManager::GetInstance();
	IList* pPersonList = pAddressbookMgr->GetAllPersonsN();
	result r = GetLastResult();
	AppLog("%S", GetErrorMessage(r));

	if (r != E_SUCCESS) {
		return;
	}

	if (__pContactsList) {
		__pContactsList->RemoveAll();
	} else {
		__pContactsList = new LinkedList();
	}

	if (pPersonList) {

		for (int i = 0; i < pPersonList->GetCount(); i++) {
			Person* pPerson = static_cast <Person*> (pPersonList->GetAt(i));
			__pContactsList->Add(MUser::CreateFromPerson(pPerson));
		}

		AppLog("pPersonList: %d", pPersonList->GetCount());
	}

	if (__pContactsList->GetCount() > 0) {
		//load
		this->SyncContacts();
	} else {
		//notify
	}
}

void
UiContactsPanel::SyncContacts() {
	if (!this->__pContactsList) {
		return;
	}

	if (this->__pContactsList && this->__pContactsList->GetCount() == 0) {
		return;
	}

	HashMap *params = new HashMap();
	params->Construct();

	String phonesString(L"");

	for (int i = 0; i < this->__pContactsList->GetCount(); i++) {
		MUser *pUser = static_cast<MUser *>(this->__pContactsList->GetAt(i));

		if (pUser->__pContactPhone && pUser->__pContactPhone->GetLength() > 0) {
			if (i != 0 && i != this->__pContactsList->GetCount() - 1) {
				phonesString.Append(L",");
			}
			phonesString.Append(pUser->__pContactPhone->GetPointer());
		}
	}

	if (phonesString.GetLength() == 0) {
		delete params;
		return;
	}

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"fields"), new String(L"photo_100,photo_50,online,is_friend,photo_200"));
	params->Add(new String(L"phones"), new String(phonesString));

	__pUserImportOperation = new RestRequestOperation(IMPORT_CONTACTS, new String(L"friends.getByPhones"), params);
	__pUserImportOperation->AddEventListener(this);
	__pUserImportOperation->SetResponseDescriptor(new RContactsDescriptor());
	RestClient::getInstance().PerformOperation(__pUserImportOperation);
}

/****************** REST *****************/

void
UiContactsPanel::OnSuccessN(RestResponse *result) {
	if (__pUserImportOperation) {
		__pUserImportOperation->AddEventListener(null);
		__pUserImportOperation = null;
	}

	if (result->GetOperationCode() == IMPORT_CONTACTS) {
		UserRestResponse *response = static_cast<UserRestResponse *>(result);

		if (response->GetUsers()) {
			for (int i = 0; i < response->GetUsers()->GetCount(); i++) {
				MUser *pRemoteUser = static_cast<MUser *>(response->GetUsers()->GetAt(i));
				for (int j = 0; j < this->__pContactsList->GetCount(); j++) {
					MUser *pLocalUser = static_cast<MUser *>(this->__pContactsList->GetAt(i));

					if ((pRemoteUser->__pContactPhone && pLocalUser->__pContactPhone) &&
							pRemoteUser->__pContactPhone->Equals(pLocalUser->__pContactPhone->GetPointer(), false))  {

						pLocalUser->SetFirstName(pRemoteUser->GetFirstName());
						pLocalUser->SetLastName(pRemoteUser->GetLastName());
						pLocalUser->__pBigPhoto = pRemoteUser->__pBigPhoto;
						pLocalUser->__isFriend = pRemoteUser->__isFriend;
						pLocalUser->__isContact = 1;
						pLocalUser->__isPending = pRemoteUser->__isPending;

						pLocalUser->__chat = pRemoteUser->__chat;

						pLocalUser->SetPhoto(pRemoteUser->GetPhoto());
						pLocalUser->SetMiniPhoto(pRemoteUser->GetMiniPhoto());
						pLocalUser->SetUid(pRemoteUser->GetUid());
						pLocalUser->SetLastSeen(pRemoteUser->GetLastSeen());
						pLocalUser->SetType(pRemoteUser->GetType());
						pLocalUser->SetIsOnline(pRemoteUser->GetIsOnline());

						break;
					}
				}
			}
		}

		if(this->__pUsersList) {
			__pUsersList->RemoveAll();
		} else {
			__pUsersList = new LinkedList();
		}

		__pUsersList->AddItems(*this->__pContactsList);
		this->RequestUiUpdate(result->GetOperationCode(), 0);

		MUserDao::getInstance().Save(__pUsersList, true);
	}

}

void
UiContactsPanel::OnErrorN(Error *error) {
	if (__pUserImportOperation) {
		__pUserImportOperation->AddEventListener(null);
		__pUserImportOperation = null;
	}
}

/************* UI DISPATCH ****************/

void
UiContactsPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == IMPORT_CONTACTS) {
		this->UpdateScreen();
	}
}

void
UiContactsPanel::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
}

/***************** SEARCH *****************/

void
UiContactsPanel::OnTextValueChanged(const Tizen::Ui::Control& source) {
	String string = this->__pSearchBar->GetText();

	if (string.GetLength() == 0) {
		this->SetUserList(MUserDao::getInstance().GetContactsN());
	} else {
		this->SetUserList(MUserDao::getInstance().SearchContacts(new String(string.GetPointer())));
	}

	SplitUsersToSections();
	this->__pListView->UpdateList();
}

void
UiContactsPanel::OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListItemStatus state) {
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

void
UiContactsPanel::OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index) {

}

void
UiContactsPanel::SplitUsersToSectionsByAlphabet() {
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
			wchar_t ltr;
			user->__pContactName->GetCharAt(0, ltr);
			String firstLetter(ltr);

			if (letter.Equals(firstLetter, false)) {

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

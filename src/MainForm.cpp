/*
 * MainForm.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#include "MainForm.h"
#include <FGraphics.h>
#include <FMedia.h>
#include <FApp.h>
#include <FBase.h>
#include "SceneRegister.h"
#include "MMessageDao.h"
#include "UiUsersPanel.h"
#include "UiUpdateConstants.h"
#include "LongPollConnection.h"

#include "AuthManager.h"
#include "RestClient.h"
#include "RUnreadCountDescriptor.h"
#include "RestRequestOperation.h"
#include "RestResponse.h"
#include "Error.h"
#include "Helper.h"
#include "RMessageSendResponse.h"
#include "UiMessagesPanel.h"

#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

#define ID_MESSAGES 101
#define ID_CONTACTS 102
#define ID_SEARCH 	103
#define ID_SETTINGS 104

#define ID_USERS_FRIENDS 105
#define ID_USERS_FRIENDS_ONLINE 	106
#define ID_USERS_CONTACTS 107

#define ID_SEARCH_DIALOGS 	108
#define ID_SEARCH_MESSAGES 	109

const int SEARCH_MESSAGE_MODE = 2343432;
const int SEARCH_DIALOG_MODE = 12343534;

MainForm::MainForm() {
	Form::Construct(FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	__isSearchMode = false;
	__searchModeCode = SEARCH_DIALOG_MODE;
	__lastSelectedIndex = 0;
	__normalIndex = 0;
	__searchIndex = 0;
	__footerIndex = 0;

	__unreadCount = 0;

	SetFormBackEventListener(this);
	__pGetUnreadCountOperation = null;
	messageItem = null;

	this->SetName(L"MainForm");
	Color *pFormBackgroundColor = new (std::nothrow) Color(0, 0, 0, 255);
	this->SetBackgroundColor(*pFormBackgroundColor);

	Color *pHeaderBackgroundColor = new (std::nothrow) Color(65, 97, 137, 255);
	Color *pHeaderTextColor = new (std::nothrow) Color(255, 255, 255, 255);

	Header* pHeader = this->GetHeader();
	__pHeader = pHeader;
	pHeader->SetStyle(HEADER_STYLE_TAB_LARGE);
	pHeader->SetItemTextColor(HEADER_ITEM_STATUS_NORMAL, *pHeaderTextColor);
	pHeader->SetItemTextColor(HEADER_ITEM_STATUS_SELECTED, *pHeaderTextColor);

	pHeader->SetItemColor(HEADER_ITEM_STATUS_NORMAL, *pHeaderBackgroundColor);
	pHeader->SetItemColor(HEADER_ITEM_STATUS_PRESSED, *pHeaderBackgroundColor);
	pHeader->SetItemColor(HEADER_ITEM_STATUS_SELECTED, *pHeaderBackgroundColor);

	pHeader->SetColor(*pHeaderBackgroundColor);
	pHeader->SetTabEditModeEnabled(false);
	pHeader->AddActionEventListener(*this);

	delete pHeaderBackgroundColor;
	delete pHeaderTextColor;
	delete pFormBackgroundColor;

	Footer *pFooter = this->GetFooter();
	pFooter->AddActionEventListener(*this);
	pFooter->SetItemColor(FOOTER_ITEM_STATUS_NORMAL, Color(23, 30, 38, 255));
	pFooter->SetItemColor(FOOTER_ITEM_STATUS_PRESSED, Color(23, 30, 38, 255));
	pFooter->SetItemColor(FOOTER_ITEM_STATUS_SELECTED, Color(23, 30, 38, 255));
	pFooter->SetItemColor(FOOTER_ITEM_STATUS_HIGHLIGHTED, Color(23, 30, 38, 255));

	pFooter->SetItemTextColor(FOOTER_ITEM_STATUS_NORMAL, Color(255, 255, 255, 255));
	pFooter->SetItemTextColor(FOOTER_ITEM_STATUS_SELECTED, Color(255, 255, 255, 255));

	FooterItem *friendsItem = new FooterItem();
	friendsItem->Construct(ID_USERS_FRIENDS);
	this->friendsItem = friendsItem;
	pFooter->AddItem(*friendsItem);

	FooterItem *friendsOnlineItem = new FooterItem();
	friendsOnlineItem->Construct(ID_USERS_FRIENDS_ONLINE);
	this->friendsOnlineItem = friendsOnlineItem;
	pFooter->AddItem(*friendsOnlineItem);

	FooterItem *contactsItem = new FooterItem();
	contactsItem->Construct(ID_USERS_CONTACTS);
	this->footerContactsItem = contactsItem;
	pFooter->AddItem(*contactsItem);

//	Image image;
//	image.Construct();
//	filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_marker.png";
//	Bitmap *badgeIcon = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
//	pHeader->SetItemBadgeIcon(0, badgeIcon);

	this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);

	this->SetContentAreaBounds(this->GetClientAreaBounds());

//	this->Invalidate(true);
	try {
		LongPollConnection::getInstance().Run();
		this->RequestUnreadCount();
	} catch (...) {

	}
}

MainForm::~MainForm() {
	messageItem = null;
	if (__pGetUnreadCountOperation) {
		__pGetUnreadCountOperation->AddEventListener(null);
	}
}

void
MainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
//	SceneManager* pSceneManager = SceneManager::GetInstance();
//	AppAssert(pSceneManager);
//	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

	if (this->__isSearchMode) {

	}
}

void
MainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager;
	UiUsersPanel* pUserPanel = null;

	if (actionId >= ID_USERS_FRIENDS) {
		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		pUserPanel = static_cast< UiUsersPanel* >(pFrame->GetControl("UiUsersPanel", true));
	}

	switch(actionId)
	{
	case ID_MESSAGES: {
		__lastSelectedIndex = 0;
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_MESSAGES_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
	}
		break;
	case ID_CONTACTS: {
		__lastSelectedIndex = 1;
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_USERS_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);
	}
		break;
	case ID_SEARCH: {
		__lastSelectedIndex = 2;
		this->SetFocus();
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);

		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_SEARCH_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
	}
		break;
	case ID_SETTINGS: {
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_SETTINGS, SCENE_TRANSITION_ANIMATION_TYPE_LEFT));
		this->GetHeader()->SetItemSelected(__lastSelectedIndex);
	}
		break;
	case ID_USERS_FRIENDS: {
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_USERS_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);

		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		pUserPanel = static_cast< UiUsersPanel* >(pFrame->GetControl("UiUsersPanel", true));

		if (pUserPanel) {
			pUserPanel->SetCurrentDisplayMode(0);
		}
	}
		break;
	case ID_USERS_FRIENDS_ONLINE: {
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_USERS_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);

		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		pUserPanel = static_cast< UiUsersPanel* >(pFrame->GetControl("UiUsersPanel", true));

		if (pUserPanel) {
			pUserPanel->SetCurrentDisplayMode(1);
		}
	}
		break;
	case ID_USERS_CONTACTS: {
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_CONTACTS_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);
	}
		break;
	case ID_SEARCH_DIALOGS: {
		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		UiMessagesPanel *pDialogsPanel = static_cast< UiMessagesPanel* >(pFrame->GetControl("UiMessagesPanel", true));
		if (pDialogsPanel) {
			this->__searchModeCode = SEARCH_DIALOG_MODE;
			pDialogsPanel->SetSearchModeCode(this->__searchModeCode);
		}
	}
		break;
	case ID_SEARCH_MESSAGES:
		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		UiMessagesPanel *pDialogsPanel = static_cast< UiMessagesPanel* >(pFrame->GetControl("UiMessagesPanel", true));
		if (pDialogsPanel) {
			this->__searchModeCode = SEARCH_MESSAGE_MODE;
			pDialogsPanel->SetSearchModeCode(this->__searchModeCode);
		}
		break;
	}
}

void
MainForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == UPDATE_MESSAGE_ARRIVED || requestId == UPDATE_READ_STATE) {
		UpdateUnreadCount();
	} else if (requestId == UPDATE_UNREAD_COUNT) {
		if (pArgs->GetCount() > 0) {
			Integer *count = static_cast<Integer *>(pArgs->GetAt(0));
			this->UpdateUnreadCount(count->ToInt());
		}
	}

	delete pArgs;
}

void
MainForm::OnSuccessN(RestResponse *user) {

	if (user->GetOperationCode() == GET_UNREAD_COUNT) {
		__pGetUnreadCountOperation->AddEventListener(null);
		__pGetUnreadCountOperation = null;

		RMessageSendResponse *result = static_cast<RMessageSendResponse *>(user);

		ArrayList *list = new ArrayList();
		list->Construct(1);

		list->Add(new Integer(result->__mid));

		this->SendUserEvent(UPDATE_UNREAD_COUNT, list);
		Tizen::App::App::GetInstance()->SendUserEvent(UPDATE_UNREAD_COUNT, 0);
	}
}

void
MainForm::OnErrorN(Error *error) {

}

void
MainForm::UpdateUnreadCount(int unreadCount) {

	int count = 0;

	if (unreadCount == -1) {
		this->RequestUnreadCount();
		return;
	}

	count = unreadCount;

	AppLogDebug("!UpdateUnreadCount %d", count);
	__unreadCount = count;

	if (count > 0) {
		__pHeader->SetItemNumberedBadgeIcon(0, 0);
		__pHeader->SetItemNumberedBadgeIcon(0, count);
	} else {
		__pHeader->SetItemNumberedBadgeIcon(0, 0);
	}
}

void
MainForm::RequestUnreadCount() {
	if (!__pGetUnreadCountOperation) {
		HashMap *params = new HashMap();
		params->Construct();
		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		String code;
		code.Append(""
			"var a = API.messages.get({\"offset\" : 0, \"count\":200, \"out\":0});"
			"var c = a.items@.read_state;"
			"var count = 0;"
			"var i = 0;"
			"while (i < c.length) { "
			"	if (parseInt(c[i]) == 0) {"
			"		count = count + 1;"
			"	}"
			"	i=i+1; "
			"};"
			"return {\"count\" : count};"
				);
		params->Add(new String(L"code"), new String(code));

		__pGetUnreadCountOperation = new RestRequestOperation(GET_UNREAD_COUNT, new String(L"execute"), params);
		__pGetUnreadCountOperation->AddEventListener(this);
		__pGetUnreadCountOperation->SetResponseDescriptor(new RUnreadCountDescriptor());
		RestClient::getInstance().PerformOperation(__pGetUnreadCountOperation);
	}
}

result
MainForm::OnDraw() {
	this->RecreateItems();
	return E_SUCCESS;
}

void
MainForm::RecreateItems() {

	Header* pHeader = this->GetHeader();
	Footer *pFooter = this->GetFooter();

	int headerIndex = 0;
	int footerIndex = 0;

	if (this->SearchModeIsActive()) {
		if (this->__searchIndex  != 0) {
			headerIndex = pHeader->GetSelectedItemIndex();
		} else {
			headerIndex = 0;
		}

		this->__searchIndex = 2;

	} else {
		if (this->__searchIndex != 0) {
			this->__searchIndex = 0;
			this->__normalIndex =  0;
			this->__footerIndex = 0;
		} else {
			this->__normalIndex =  pHeader->GetSelectedItemIndex();
			this->__footerIndex = pFooter->GetSelectedItemIndex();
		}

		headerIndex = this->__normalIndex;
		footerIndex = this->__footerIndex;
	}

	pHeader->RemoveAllItems();
	pFooter->RemoveAllItems();

	if (this->__isSearchMode) {
		HeaderItem *headerSearchDialogsItem = new HeaderItem();
		headerSearchDialogsItem->Construct(ID_SEARCH_DIALOGS);
		String dialogsString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH_DIALOGS, dialogsString);
		headerSearchDialogsItem->SetText(dialogsString);
		pHeader->AddItem(*headerSearchDialogsItem);

		HeaderItem *headerSearchMessageItem = new HeaderItem();
		headerSearchMessageItem->Construct(ID_SEARCH_MESSAGES);
		String messageString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH_MESSAGES, messageString);
		headerSearchMessageItem->SetText(messageString);
		pHeader->AddItem(*headerSearchMessageItem);
	} else {
		Image messageIcon;
		result r = messageIcon.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_message.png";
		Bitmap *pMessagesIconBitmap = messageIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

		HeaderItem *headerMessageItem = new HeaderItem();
		headerMessageItem->Construct(ID_MESSAGES);
		headerMessageItem->SetIcon(HEADER_ITEM_STATUS_NORMAL, pMessagesIconBitmap);
		messageItem = headerMessageItem;
		String messageString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_MSG, messageString);
		this->messageItem->SetText(messageString);
		pHeader->AddItem(*headerMessageItem);

		pHeader->SetItemNumberedBadgeIcon(0, this->__unreadCount);

		Image contactsIcon;
		r = contactsIcon.Construct();
		filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_contacts.png";
		Bitmap *pContactsIconBitmap = contactsIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

		HeaderItem *headerContactsItem = new HeaderItem();
		headerContactsItem->Construct(ID_CONTACTS);
		headerContactsItem->SetIcon(HEADER_ITEM_STATUS_NORMAL, pContactsIconBitmap);
		contactsItem = headerContactsItem;

		String contactsString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_CONTACTS, contactsString);
		this->contactsItem->SetText(contactsString);

		pHeader->AddItem(*headerContactsItem);

		Image searchIcon;
		r = searchIcon.Construct();
		filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_search.png";
		Bitmap *pSearchIconBitmap = searchIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

		HeaderItem *headerSearchItem = new HeaderItem();
		headerSearchItem->Construct(ID_SEARCH);
		headerSearchItem->SetIcon(HEADER_ITEM_STATUS_NORMAL, pSearchIconBitmap);
		searchItem = headerSearchItem;
		String searchString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_SEARCH, searchString);
		this->searchItem->SetText(searchString);
		pHeader->AddItem(*headerSearchItem);

		Image settingsIcon;
		r = settingsIcon.Construct();
		filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_gear.png";
		Bitmap *pSettingsIconBitmap = settingsIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

		HeaderItem *headerSettingsItem = new HeaderItem();
		headerSettingsItem->Construct(ID_SETTINGS);
		headerSettingsItem->SetIcon(HEADER_ITEM_STATUS_NORMAL, pSettingsIconBitmap);
		settingsItem = headerSettingsItem;
		String settingsString;
		Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_SETTINGS, settingsString);
		this->settingsItem->SetText(settingsString);
		pHeader->AddItem(*headerSettingsItem);

		delete pMessagesIconBitmap;
		delete pContactsIconBitmap;
		delete pSearchIconBitmap;
		delete pSettingsIconBitmap;
	}

	FooterItem *friendsItem = new FooterItem();
	friendsItem->Construct(ID_USERS_FRIENDS);
	this->friendsItem = friendsItem;
	String allFriendsString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_FRIENDS, allFriendsString);
	this->friendsItem->SetText(allFriendsString);

	pFooter->AddItem(*friendsItem);

	FooterItem *friendsOnlineItem = new FooterItem();
	friendsOnlineItem->Construct(ID_USERS_FRIENDS_ONLINE);
	this->friendsOnlineItem = friendsOnlineItem;
	String onlineString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_FRIENDS_ONLINE, onlineString);
	this->friendsOnlineItem->SetText(onlineString);
	pFooter->AddItem(*friendsOnlineItem);

	FooterItem *contactsItem = new FooterItem();
	contactsItem->Construct(ID_USERS_CONTACTS);
	this->footerContactsItem = contactsItem;
	String importContactsString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_MAIN_FORM_I_CONTACTS, importContactsString);
	this->footerContactsItem->SetText(importContactsString);

	pFooter->AddItem(*contactsItem);

	pHeader->SetItemSelected(headerIndex);
	pFooter->SetItemSelected(footerIndex);
}

void
MainForm::SetSearchMode(bool state) {
	__isSearchMode = state;

	this->RequestRedraw(true);
}

bool
MainForm::SearchModeIsActive() {
	return __isSearchMode;
}

void
MainForm::SetSearchModeCode(int code) {
	__searchModeCode = code;
}

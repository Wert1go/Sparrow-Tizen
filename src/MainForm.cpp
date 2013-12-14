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

MainForm::MainForm() {
	Form::Construct(FORM_STYLE_HEADER | FORM_STYLE_FOOTER);
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
		SAFE_DELETE(__pGetUnreadCountOperation);
	}
}

void
MainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
//	SceneManager* pSceneManager = SceneManager::GetInstance();
//	AppAssert(pSceneManager);
//	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
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
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_MESSAGES_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
	}
		break;
	case ID_CONTACTS: {
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_USERS_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);
	}
		break;
	case ID_SEARCH:
		this->SetFocus();
		this->SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);

		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_SEARCH_TAB, SCENE_TRANSITION_ANIMATION_TYPE_NONE,
					SCENE_HISTORY_OPTION_NO_HISTORY));

		break;
	case ID_SETTINGS:
		this->SetFocus();
		pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_SETTINGS, SCENE_TRANSITION_ANIMATION_TYPE_LEFT));
		break;
	case ID_USERS_FRIENDS:
		if (pUserPanel) {
			pUserPanel->SetCurrentDisplayMode(0);
		}

		break;
	case ID_USERS_FRIENDS_ONLINE:
		if (pUserPanel) {
			pUserPanel->SetCurrentDisplayMode(1);
		}
		break;
	case ID_USERS_CONTACTS:
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

	int count = MMessageDao::getInstance().GetUnreadCount();

	if (count != unreadCount) {
		count = unreadCount;
	}

	AppLogDebug("!UpdateUnreadCount %d", count);
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
			"	i=i+1; "
			"	if (parseInt(c[i]) == 0) {"
			"		count = count + 1;"
			"	}"
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

	if (messageItem) {
		headerIndex = pHeader->GetSelectedItemIndex();
		footerIndex = pFooter->GetSelectedItemIndex();

		pHeader->RemoveAllItems();
		pFooter->RemoveAllItems();

		//delete
	}

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

	AppLog("headerIndex %d footerIndex %d", headerIndex, footerIndex);

	pHeader->SetItemSelected(headerIndex);
	pFooter->SetItemSelected(footerIndex);
}

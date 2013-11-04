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

MainForm::MainForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);

	Color *pFormBackgroundColor = new (std::nothrow) Color(0, 0, 0, 255);
	this->SetBackgroundColor(*pFormBackgroundColor);

	Color *pHeaderBackgroundColor = new (std::nothrow) Color(65, 97, 137, 255);
	Color *pHeaderTextColor = new (std::nothrow) Color(255, 255, 255, 255);

	Header* pHeader = this->GetHeader();
	pHeader->SetTabEditModeEnabled(false);
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

	Image messageIcon;
	result r = messageIcon.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_message.png";
	Bitmap *pMessagesIconBitmap = messageIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	HeaderItem headerMessageItem;
	headerMessageItem.Construct(ID_MESSAGES);
	headerMessageItem.SetText("Сообщения");
	headerMessageItem.SetIcon(HEADER_ITEM_STATUS_NORMAL, pMessagesIconBitmap);
	pHeader->AddItem(headerMessageItem);

	Image contactsIcon;
	r = contactsIcon.Construct();
	filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_contacts.png";
	Bitmap *pContactsIconBitmap = contactsIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	HeaderItem headerContactsItem;
	headerContactsItem.Construct(ID_CONTACTS);
	headerContactsItem.SetText("Контакты");
	headerContactsItem.SetIcon(HEADER_ITEM_STATUS_NORMAL, pContactsIconBitmap);
	pHeader->AddItem(headerContactsItem);

	Image searchIcon;
	r = searchIcon.Construct();
	filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_search.png";
	Bitmap *pSearchIconBitmap = searchIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	HeaderItem headerSearchItem;
	headerSearchItem.Construct(ID_SEARCH);
	headerSearchItem.SetText("Поиск");
	headerSearchItem.SetIcon(HEADER_ITEM_STATUS_NORMAL, pSearchIconBitmap);
	pHeader->AddItem(headerSearchItem);

	Image settingsIcon;
	r = settingsIcon.Construct();
	filepath = App::GetInstance()->GetAppResourcePath() + L"Images/tab_icon_gear.png";
	Bitmap *pSettingsIconBitmap = settingsIcon.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	HeaderItem headerSettingsItem;
	headerSettingsItem.Construct(ID_SETTINGS);
	headerSettingsItem.SetText("Настройки");
	headerSettingsItem.SetIcon(HEADER_ITEM_STATUS_NORMAL, pSettingsIconBitmap);
	pHeader->AddItem(headerSettingsItem);

	delete pMessagesIconBitmap;
	delete pContactsIconBitmap;
	delete pSearchIconBitmap;
	delete pSettingsIconBitmap;
}

MainForm::~MainForm() {
	// TODO Auto-generated destructor stub
}

void
MainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
MainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{

	switch(actionId)
	{
	case ID_MESSAGES:

		break;
	case ID_CONTACTS:

		break;
	case ID_SEARCH:

		break;
	case ID_SETTINGS:
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_SETTINGS, SCENE_TRANSITION_ANIMATION_TYPE_LEFT));
		break;
	}
}

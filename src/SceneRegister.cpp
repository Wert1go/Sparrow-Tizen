/*
 * SceneRegister.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#include "SceneRegister.h"
#include <FUi.h>
#include "FormFactory.h"
#include "PanelFactory.h"

using namespace Tizen::Ui::Scenes;

const wchar_t* SCENE_AUTH = L"AUTH_SCENE";
const wchar_t* SCENE_WEB_AUTH = L"WEB_AUTH_SCENE";
const wchar_t* SCENE_MAIN = L"MAIN_SCENE";
const wchar_t* SCENE_CHAT = L"SCENE_CHAT";
const wchar_t* SCENE_MAIN_MESSAGES_TAB = L"SCENE_MAIN_MESSAGES_TAB";
const wchar_t* SCENE_MAIN_USERS_TAB = L"SCENE_MAIN_USERS_TAB";
const wchar_t* SCENE_MAIN_SEARCH_TAB = L"SCENE_MAIN_SEARCH_TAB";
const wchar_t* SCENE_SETTINGS = L"SCENE_SETTINGS";
const wchar_t* SCENE_NEW_MESSAGE = L"SCENE_NEW_MESSAGE";
const wchar_t* SCENE_SELECT_USER = L"SCENE_SELECT_USER";
const wchar_t* SCENE_CREATE_CONV = L"SCENE_CREATE_CONV";
const wchar_t* SCENE_PROFILE = L"SCENE_PROFILE";

const wchar_t* SCENE_IMAGE_VIEWER = L"SCENE_IMAGE_VIEWER";
const wchar_t* SCENE_VIDEO_VIEWER = L"SCENE_VIDEO_VIEWER";



SceneRegister::SceneRegister() {
	// TODO Auto-generated constructor stub

}

SceneRegister::~SceneRegister() {
	// TODO Auto-generated destructor stub
}

void
SceneRegister::RegisterAllScenes(void)
{
	static const wchar_t* PANEL_BLANK = L"";
	static FormFactory formFactory;
	static PanelFactory panelFactory;

	SceneManager* pSceneManager = SceneManager::GetInstance();
	pSceneManager->RegisterFormFactory(formFactory);
	pSceneManager->RegisterPanelFactory(panelFactory);

	pSceneManager->RegisterScene(SCENE_AUTH, FORM_AUTH, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_WEB_AUTH, FORM_WEB_AUTH, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_MAIN, FORM_MAIN, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_CHAT, FORM_CHAT, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_SETTINGS, FORM_SETTINGS, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_MAIN_MESSAGES_TAB, FORM_MAIN, PANEL_DIALOGS);
	pSceneManager->RegisterScene(SCENE_MAIN_USERS_TAB, FORM_MAIN, PANEL_USERS);
	pSceneManager->RegisterScene(SCENE_MAIN_SEARCH_TAB, FORM_MAIN, PANEL_SEARCH);
	pSceneManager->RegisterScene(SCENE_NEW_MESSAGE, FORM_NEW_MESSAGE, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_SELECT_USER, FORM_SELECT_USER, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_CREATE_CONV, FORM_CREATE_CONV, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_PROFILE,FORM_PROFILE, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_IMAGE_VIEWER, FORM_IMAGE_VIEWER, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_VIDEO_VIEWER, FORM_VIDEO_VIEWER, PANEL_BLANK);


}


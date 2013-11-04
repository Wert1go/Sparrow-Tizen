/*
 * SceneRegister.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#include "SceneRegister.h"
#include <FUi.h>
#include "FormFactory.h"

using namespace Tizen::Ui::Scenes;

const wchar_t* SCENE_AUTH = L"AUTH_SCENE";
const wchar_t* SCENE_WEB_AUTH = L"WEB_AUTH_SCENE";
const wchar_t* SCENE_MAIN = L"MAIN_SCENE";
const wchar_t* SCENE_SETTINGS = L"SCENE_SETTINGS";

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

	SceneManager* pSceneManager = SceneManager::GetInstance();
	pSceneManager->RegisterFormFactory(formFactory);

	pSceneManager->RegisterScene(SCENE_AUTH, FORM_AUTH, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_WEB_AUTH, FORM_WEB_AUTH, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_MAIN, FORM_MAIN, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_SETTINGS, FORM_SETTINGS, PANEL_BLANK);
}


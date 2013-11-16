/*
 * FormFactory.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#include "FormFactory.h"
#include "AuthForm.h"
#include "AuthWebForm.h"
#include "MainForm.h"
#include "SettingsForm.h"
#include "UiChatForm.h"

using namespace Tizen::Ui::Scenes;

const wchar_t* FORM_AUTH = L"FORM_AUTH";
const wchar_t* FORM_WEB_AUTH = L"FORM_WEB_AUTH";
const wchar_t* FORM_MAIN = L"FORM_MAIN";
const wchar_t* FORM_SETTINGS = L"FORM_SETTINGS";
const wchar_t* FORM_CHAT = L"FORM_CHAT";


const wchar_t* FORM_TAB_STYLE_BASE = L"FromTabStyleBase";

FormFactory::FormFactory() {
	// TODO Auto-generated constructor stub

}

FormFactory::~FormFactory() {
	// TODO Auto-generated destructor stub
}

Tizen::Ui::Controls::Form*
FormFactory::CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	Tizen::Ui::Controls::Form* pNewForm = null;
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	if (formId == FORM_AUTH)
	{
		AuthForm* pForm = new (std::nothrow) AuthForm();
		pNewForm = pForm;
	}
	else if (formId == FORM_WEB_AUTH)
	{
		AuthWebForm* pForm = new (std::nothrow) AuthWebForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_MAIN)
	{
		MainForm *pForm = new (std::nothrow) MainForm();
		pNewForm = pForm;
	}
	else if (formId == FORM_SETTINGS)
	{
		SettingsForm *pForm = new (std::nothrow) SettingsForm();
		pNewForm = pForm;
	}
	else if (formId == FORM_CHAT)
	{
		UiChatForm *pForm = new (std::nothrow) UiChatForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}

	return pNewForm;
}


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
#include "CreateChatForm.h"
#include "SelectChatUsersForm.h"
#include "CreateConversationForm.h"
#include "ProfileForm.h"
#include "UiChatEditForm.h"

#include "UiImageViewer.h"
#include "UiVideoViewer.h"

using namespace Tizen::Ui::Scenes;

const wchar_t* FORM_AUTH = L"FORM_AUTH";
const wchar_t* FORM_WEB_AUTH = L"FORM_WEB_AUTH";
const wchar_t* FORM_MAIN = L"FORM_MAIN";
const wchar_t* FORM_SETTINGS = L"FORM_SETTINGS";
const wchar_t* FORM_CHAT = L"FORM_CHAT";
const wchar_t* FORM_NEW_MESSAGE = L"FORM_NEW_MESSAGE";
const wchar_t* FORM_SELECT_USER = L"FORM_SELECT_USER";
const wchar_t* FORM_CREATE_CONV = L"FORM_CREATE_CONV";
const wchar_t* FORM_PROFILE = L"FORM_PROFILE";

const wchar_t* FORM_IMAGE_VIEWER = L"FORM_IMAGE_VIEWER";
const wchar_t* FORM_VIDEO_VIEWER = L"FORM_VIDEO_VIEWER";

const wchar_t* FORM_CHAT_EDIT = L"FORM_CHAT_EDIT";



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
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_CHAT)
	{
		UiChatForm *pForm = new (std::nothrow) UiChatForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_NEW_MESSAGE) {
		CreateChatForm *pForm = new (std::nothrow) CreateChatForm();

		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_SELECT_USER) {
		SelectChatUsersForm *pForm = new (std::nothrow) SelectChatUsersForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_CREATE_CONV) {
		CreateConversationForm *pForm = new (std::nothrow) CreateConversationForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_PROFILE) {
		ProfileForm *pForm = new (std::nothrow) ProfileForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_IMAGE_VIEWER) {
		UiImageViewer *pForm = new (std::nothrow) UiImageViewer();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_VIDEO_VIEWER) {
		UiVideoViewer *pForm = new (std::nothrow) UiVideoViewer();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_CHAT_EDIT) {
		UiChatEditForm *pForm = new (std::nothrow) UiChatEditForm();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}

	return pNewForm;
}


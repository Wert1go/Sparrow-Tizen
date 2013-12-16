/*
 * PanelFactory.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include "PanelFactory.h"
#include "UiMessagesPanel.h"
#include "UiUsersPanel.h"
#include "SearchUserPanel.h"
#include "UiContactsPanel.h"

#include <FUi.h>

using namespace Tizen::Ui;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Ui::Controls;

const wchar_t* PANEL_DIALOGS = L"PANEL_DIALOGS";
const wchar_t* PANEL_USERS = L"PANEL_USERS";
const wchar_t* PANEL_SEARCH = L"PANEL_SEARCH";
const wchar_t* PANEL_CONTACTS = L"PANEL_CONTACTS";

PanelFactory::PanelFactory() {
	// TODO Auto-generated constructor stub

}

PanelFactory::~PanelFactory() {
	// TODO Auto-generated destructor stub
}

Tizen::Ui::Controls::Panel*
PanelFactory::CreatePanelN(const Tizen::Base::String& panelId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	Panel*	pNewPanel = null;
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	if (panelId == PANEL_DIALOGS)
	{
		UiMessagesPanel* pPanel = new (std::nothrow) UiMessagesPanel();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	} else if (panelId == PANEL_USERS) {
		UiUsersPanel* pPanel = new (std::nothrow) UiUsersPanel();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	} else if (panelId == PANEL_SEARCH) {
		SearchUserPanel* pPanel = new (std::nothrow) SearchUserPanel();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	} else if (panelId == PANEL_CONTACTS) {
		UiContactsPanel* pPanel = new (std::nothrow) UiContactsPanel();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}

	return pNewPanel;
}


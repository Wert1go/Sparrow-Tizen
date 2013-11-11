/*
 * PanelFactory.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include "PanelFactory.h"
#include "UiMessagesPanel.h"

#include <FUi.h>

using namespace Tizen::Ui;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Ui::Controls;

const wchar_t* PANEL_DIALOGS = L"PANEL_DIALOGS";

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
		//pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}

	return pNewPanel;
}


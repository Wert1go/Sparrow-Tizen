/**
 * Name        : SparrowFrame
 * Version     :
 * Vendor      :
 * Description :
 */


#include "SparrowFrame.h"
#include "SceneRegister.h"
#include "AuthManager.h"
#include "RestClient.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

SparrowFrame::SparrowFrame(void)
{
}

SparrowFrame::~SparrowFrame(void)
{
}

result
SparrowFrame::OnInitializing(void)
{
	result r = E_SUCCESS;
	String *baseUrl = new String(L"https://api.vk.com/");
	RestClient::getInstance().SetBaseUrl(baseUrl);

	SceneRegister::RegisterAllScenes();

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	if (AuthManager::getInstance().IsAuthorized()) {
		r = pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_MESSAGES_TAB));
	} else {
		r = pSceneManager->GoForward(ForwardSceneTransition(SCENE_AUTH));
	}

	return r;
}

result
SparrowFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO:
	// Add your termination code here
	return r;
}



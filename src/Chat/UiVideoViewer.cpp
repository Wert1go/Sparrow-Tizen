/*
 * UiVideoViewer.cpp
 *
 *  Created on: Dec 14, 2013
 *      Author: developer
 */

#include "UiVideoViewer.h"
#include <FApp.h>
#include "Error.h"
#include "MAttachment.h"
#include "RestClient.h"
#include "RestRequestOperation.h"
#include "AuthManager.h"

#include "RMessageSendDescriptor.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;

UiVideoViewer::UiVideoViewer() {
	__pLoadingVideoInfoOperation = null;
}

UiVideoViewer::~UiVideoViewer() {
	if (__pLoadingVideoInfoOperation) {
		delete __pLoadingVideoInfoOperation;
		__pLoadingVideoInfoOperation = null;
	}
}

bool
UiVideoViewer::Initialize(void) {
	result r = E_SUCCESS;
	r = Form::Construct(FORM_STYLE_NORMAL);
	this->SetName(L"UiVideoViewer");
	SetFormBackEventListener(this);
	return true;
}

result
UiVideoViewer::OnInitializing(void) {
	result r = E_SUCCESS;




	return r;
}

result
UiVideoViewer::OnTerminating(void) {
	result r = E_SUCCESS;

	return r;
}

void
UiVideoViewer::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs && pArgs->GetCount() > 0) {

		MAttachment *pAttachment = static_cast<MAttachment *>(pArgs->GetAt(0));

		HashMap *params = new HashMap();
		String owner;
		owner.Format(25, L"%d", pAttachment->__ownerId);
		String vid;
		vid.Format(25, L"%d", pAttachment->__id);

		owner.Append(L"_");
		owner.Append(vid);

		AppLog("owner: %S", owner.GetPointer());

		params->Construct();
		params->Add(new String(L"videos"), new String(owner));
		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		if (!__pLoadingVideoInfoOperation) {
			__pLoadingVideoInfoOperation = new RestRequestOperation(VIDEO_INFO_LOADING, new String(L"video.get"), params);
			__pLoadingVideoInfoOperation->AddEventListener(this);
			__pLoadingVideoInfoOperation->SetResponseDescriptor(new RMessageSendDescriptor());
			//RestClient::getInstance().PerformOperation(__pLoadingVideoInfoOperation);
		}
	}

}

void
UiVideoViewer::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
UiVideoViewer::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
UiVideoViewer::OnSuccessN(RestResponse *result) {

}

void
UiVideoViewer::OnErrorN(Error *error) {

}


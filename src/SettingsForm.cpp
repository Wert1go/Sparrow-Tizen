/*
 * SettingsForm.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#include "SettingsForm.h"
#include <FGraphics.h>
#include <FMedia.h>
#include <FApp.h>
#include <FBase.h>
#include <FNet.h>
#include <FWeb.h>
#include "SceneRegister.h"
#include "Util.h"

#include "MUser.h"
#include "RestRequestOperation.h"
#include "UserDescriptor.h"
#include "UserRestResponse.h"
#include "RestClient.h"
#include "AuthManager.h"

#include "MUserDao.h"
#include "ImageCache.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Media;
using namespace Tizen::Net::Http;
using namespace Tizen::Web::Json;


SettingsForm::SettingsForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);

	Color *pFormBackgroundColor = new (std::nothrow) Color(0, 0, 0, 255);
	this->SetBackgroundColor(*pFormBackgroundColor);

	Color *pHeaderBackgroundColor = new (std::nothrow) Color(65, 97, 137, 255);
	Color *pHeaderTextColor = new (std::nothrow) Color(255, 255, 255, 255);

	Header* pHeader = this->GetHeader();

	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetColor(*pHeaderBackgroundColor);
	pHeader->SetTitleTextColor(*pHeaderTextColor);
	pHeader->SetTitleText(L"Имя пользователя");

	delete pHeaderBackgroundColor;
	delete pHeaderTextColor;
	delete pFormBackgroundColor;
	__user = null;
	__userRequestOperation = null;

	__bitmap = null;

	String *uidString = AuthManager::getInstance().UserId();

	int uid;
	Integer::Parse(uidString->GetPointer(), uid);

	MUser *user = MUserDao::getInstance().GetUserN(uid);
	if (user) {
		__user = user;
		UpdateInterfaceForCurrentUser();
	} else {
		SendRequest();
	}
}

SettingsForm::~SettingsForm() {
	if (__user) {
		delete __user;
	}

	if (__userRequestOperation) {
		__userRequestOperation->AddEventListener(null);
	}

	ImageCache::getInstance().CancelLoadingForTarget(this);

	delete __bitmap;
}

void
SettingsForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
SettingsForm::SendRequest() {

	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"user_ids"), AuthManager::getInstance().UserId());
	params->Add(new String(L"fields"), new String(L"photo_50,photo_100,last_seen,online"));

	if (!__userRequestOperation) {
		__userRequestOperation = new RestRequestOperation(GET_USER, new String(L"users.get"), params);
		__userRequestOperation->AddEventListener(this);
		__userRequestOperation->SetResponseDescriptor(new UserDescriptor());
		RestClient::getInstance().PerformOperation(__userRequestOperation);
	}
}

void SettingsForm::OnSuccessN(RestResponse *response) {

	if (response->GetOperationCode() == GET_USER) {

		if(__userRequestOperation) {
			__userRequestOperation->AddEventListener(null);
			__userRequestOperation = null;
		}

		AppLogDebug("OnSuccessNOnSuccessNOnSuccessNOnSuccessNOnSuccessN");

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);

		MUser *user = userResponse->GetUser();

		if (user != null && user->GetFirstName() != null) {
			__user = user;
		}
	}

	this->SendUserEvent(response->GetOperationCode(), 0);
	Tizen::App::App::GetInstance()->SendUserEvent(response->GetOperationCode(), 0);
}

void SettingsForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

	if (requestId == GET_USER) {
		MUser *user = MUserDao::getInstance().GetUserN(__user->GetUid());
		__user = user;

		UpdateInterfaceForCurrentUser();
	}
	delete pArgs;
}

void SettingsForm::OnErrorN(Error *error) {
	if (__userRequestOperation) {
		__userRequestOperation->AddEventListener(null);
		__userRequestOperation = null;
	}

	delete error;
}

void SettingsForm::UpdateInterfaceForCurrentUser() {
	if (!__user) {
		return;
	}

	String title(L"");
	title.Append(__user->GetFirstName()->GetPointer());
	title.Append(L" ");
	title.Append(__user->GetLastName()->GetPointer());


	ImageCache::getInstance().LoadImageForTarget(__user->GetPhoto(), this);

	this->GetHeader()->SetTitleText(title);
	Draw();
}


void SettingsForm::OnImageLoadedN(Bitmap *result) {
	__bitmap = result;
	OnDraw();
}


result SettingsForm::OnDraw() {
	result r = E_SUCCESS;

	Canvas* pCanvas = GetCanvasN();

	if (pCanvas != null && __bitmap != null)
	{
		Rectangle bounds = GetBounds();

		Rectangle rect = Rectangle(bounds.width/2 - 100/2, 300, 100, 100);
		r = pCanvas->DrawBitmap(rect, *__bitmap);
	}

	delete pCanvas;

	return r;
}




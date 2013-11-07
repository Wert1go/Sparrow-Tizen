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

#include "User.h"
#include "RestRequestOperation.h"
#include "UserDescriptor.h"
#include "UserRestResponse.h"
#include "RestClient.h"
#include "AuthManager.h"


using namespace Tizen::App;
using namespace Tizen::Base;
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
	SendRequest();
}

SettingsForm::~SettingsForm() {
	if (__user) {
		delete __user;
	}

	if (__userRequestOperation) {
		__userRequestOperation->AddEventListener(null);
		AppLogDebug("11111111111~SettingsForm~SettingsForm~SettingsForm~SettingsForm");
	}

	AppLogDebug("~SettingsForm~SettingsForm~SettingsForm~SettingsForm");
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
	params->Add(new String(L"fields"), new String(L"photo_100"));

	if (!__userRequestOperation) {
		__userRequestOperation = new RestRequestOperation(GET_USER, new String(L"users.get"), params);
		__userRequestOperation->AddEventListener(this);
		__userRequestOperation->SetResponseDescriptor(new UserDescriptor());
		RestClient::getInstance().PerformOperation(__userRequestOperation);
	}
}

void SettingsForm::OnSuccessN(RestResponse *response) {

	if (response->GetOperationCode() == GET_USER) {

		__userRequestOperation->AddEventListener(null);
		__userRequestOperation = null;

		AppLogDebug("OnSuccessNOnSuccessNOnSuccessNOnSuccessNOnSuccessN");

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);

		User *user = userResponse->GetUser();

		if (user != null && user->GetFirstName() != null) {
			__user = user;
		}
	}

	this->SendUserEvent(response->GetOperationCode(), 0);
	Tizen::App::App::GetInstance()->SendUserEvent(response->GetOperationCode(), 0);
}

void SettingsForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

	if (requestId == GET_USER) {
		User *user = __user;

		String title(L"");

		//AppLog("OnTransactionReadyToRead2");

		title.Append(user->GetFirstName()->GetPointer());
		title.Append(L" ");
		title.Append(user->GetLastName()->GetPointer());
		this->GetHeader()->SetTitleText(title);
		Draw();

		//AppLog("OnTransactionReadyToRead3");
	}
	delete pArgs;
}

void SettingsForm::OnErrorN(Error *error) {
	__userRequestOperation->AddEventListener(null);
	__userRequestOperation = null;

	delete error;
}







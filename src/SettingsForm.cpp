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

	pHeader->SetTabEditModeEnabled(false);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetColor(*pHeaderBackgroundColor);
	pHeader->SetTitleTextColor(*pHeaderTextColor);
	pHeader->SetTitleText(L"Имя пользователя");

	delete pHeaderBackgroundColor;
	delete pHeaderTextColor;
	delete pFormBackgroundColor;

	SendRequest();
}

SettingsForm::~SettingsForm() {
	// TODO Auto-generated destructor stub
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

	RestRequestOperation *operation = new RestRequestOperation();
	operation->AddEventListener(this);
	operation->SetResponseDescriptor(new UserDescriptor());
	operation->perform();
}

void SettingsForm::OnSuccessN(RestResponse *response) {

	UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);

	User *user = userResponse->GetUser();

	if (user != null && user->GetFirstName() != null) {

		__user = user;

		this->SendUserEvent(0, 0);
		Tizen::App::App::GetInstance()->SendUserEvent(0, 0);
	}
}

void SettingsForm::OnUserEventReceivedN	(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

	User *user = __user;

	String title(L"");

	//AppLog("OnTransactionReadyToRead2");

	title.Append(user->GetFirstName()->GetPointer());
	title.Append(L" ");
	title.Append(user->GetLastName()->GetPointer());
	this->GetHeader()->SetTitleText(title);
	Draw();

	//AppLog("OnTransactionReadyToRead3");

	delete pArgs;
}

void SettingsForm::OnErrorN() {

}







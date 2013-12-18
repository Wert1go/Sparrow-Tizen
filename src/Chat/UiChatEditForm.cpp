/*
 * UiChatEditForm.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: developer
 */

#include "UiChatEditForm.h"

#include <FBase.h>
#include <FApp.h>

#include "RestRequestOperation.h"
#include "MDialog.h"
#include "RestResponse.h"
#include "RestClient.h"

#include "RMessageSendDescriptor.h"
#include "RMessageSendResponse.h"
#include "AuthManager.h"

#include "AppResourceId.h"

#include "MDialogDao.h"


using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiChatEditForm::UiChatEditForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);

	__pQuitConversationOperation = null;
}

UiChatEditForm::~UiChatEditForm() {
	if (__pQuitConversationOperation) {
		__pQuitConversationOperation->AddEventListener(null);
		__pQuitConversationOperation = null;
	}
}

result
UiChatEditForm::OnInitializing(void) {

	Rectangle rect = GetBounds();
	int buttonWidth = 0;

	buttonWidth = (rect.width * 94)/100;

	Color *textNormalColor = new (std::nothrow) Color(255, 255, 255, 255);
	Color *textSelectedColor = new (std::nothrow) Color(0, 0, 0, 255);
	Color *buttonNormalColor = new (std::nothrow) Color(52, 87, 119, 255);
	Color *buttonSelectedColor = new (std::nothrow) Color(255, 255, 255, 255);

	__pQuitButton = new (std::nothrow) Button();
	__pQuitButton->Construct(Rectangle(rect.width/2 - buttonWidth/2, rect.height/2 - 80/2, buttonWidth, 80));

	__pQuitButton->SetActionId(23);
	__pQuitButton->AddActionEventListener(*this);
	__pQuitButton->SetTextColor(*textNormalColor);
	__pQuitButton->SetHighlightedTextColor(*textSelectedColor);
	__pQuitButton->SetColor(BUTTON_STATUS_NORMAL, *buttonNormalColor);
	__pQuitButton->SetColor(BUTTON_STATUS_PRESSED, *buttonSelectedColor);

	AddControl(__pQuitButton);

	delete textNormalColor;
	delete textSelectedColor;
	delete buttonNormalColor;
	delete buttonSelectedColor;

	return E_SUCCESS;
}

result
UiChatEditForm::OnTerminating(void) {

	return E_SUCCESS;
}

void
UiChatEditForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs && pArgs->GetCount() > 0) {
		__pDialog = static_cast<MDialog *>(pArgs->GetAt(0));

		if (__pDialog->GetTitle()) {
			this->GetHeader()->SetTitleText(__pDialog->GetTitle()->GetPointer());
		}
	}

//	delete pArgs;
}

void
UiChatEditForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
UiChatEditForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {

	if (requestId == LEAVE_CONV) {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);

		LinkedList *params = new LinkedList();
		params->Add(new Integer(-1));
		pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT), params);
	}
}

void
UiChatEditForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	QuitConversation();
}

void
UiChatEditForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
UiChatEditForm::OnSuccessN(RestResponse *result) {
	if (__pQuitConversationOperation) {
		__pQuitConversationOperation->AddEventListener(null);
		__pQuitConversationOperation = null;
	}

	if (result->GetOperationCode() == LEAVE_CONV) {
		RMessageSendResponse *response = static_cast<RMessageSendResponse *>(result);

		if (response->__mid == 1) {
			MDialogDao::getInstance().DeleteDialog(this->__pDialog->GetUid());
			this->SendUserEvent(LEAVE_CONV, 0);
			Tizen::App::App::GetInstance()->SendUserEvent(LEAVE_CONV, 0);
		}
	}

}

void
UiChatEditForm::QuitConversation() {
	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	String chatId;
	chatId.Format(10, L"%d", this->__pDialog->GetChatId());
	params->Add(new String(L"chat_id"), new String(chatId));
	params->Add(new String(L"user_id"), AuthManager::getInstance().UserId());


	if (!__pQuitConversationOperation) {
		__pQuitConversationOperation = new RestRequestOperation(LEAVE_CONV, new String(L"messages.removeChatUser"), params);
		__pQuitConversationOperation->AddEventListener(this);
		__pQuitConversationOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__pQuitConversationOperation);
	}
}

result
UiChatEditForm::OnDraw(void) {

	String descrString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_CONV_LEAVE, descrString);
	__pQuitButton->SetText(descrString);

	return E_SUCCESS;
}

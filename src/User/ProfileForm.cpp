/*
 * ProfileForm.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#include "ProfileForm.h"

#include "MUser.h"
#include "RestRequestOperation.h"
#include "ImageCache.h"

#include <FBase.h>
#include <FApp.h>
#include <FMedia.h>

#include "MUser.h"
#include "MUserDao.h"
#include "MDialog.h"
#include "MDialogDao.h"

#include "UpdateUnit.h"
#include "Resources.h"
#include "SceneRegister.h"
#include "UiDialogCustomItem.h"
#include "CustomGroupItem.h"
#include "RestRequestOperation.h"

#include "RMessageSendDescriptor.h"
#include "RMessageSendResponse.h"
#include "AuthManager.h"
#include "RestClient.h"
#include "RestResponse.h"
#include "Error.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;

using namespace Tizen::Io;
using namespace Tizen::Media;

const int imageOffset = 100;
const int imageSize = 300;
const int imageBottomOffset = 100;

ProfileForm::ProfileForm() {
	__pUser = null;
	__pUserAvatar = null;
	__pAddToFriendsOperation = null;
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);
}

ProfileForm::~ProfileForm() {
	ImageCache::getInstance().CancelLoadingForTarget(this);
}

result
ProfileForm::OnInitializing(void) {

	Rectangle rect = this->GetClientAreaBounds();

	int buttonWidth = 0;
	int yOffset = imageOffset + imageSize + imageBottomOffset;

	this->SetBackgroundColor(Color(8, 8, 8, 255));

	buttonWidth = (rect.width * 84)/100;

	Header *pHeader = this->GetHeader();
	pHeader->SetColor(Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(33, 63, 99, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(33, 63, 99, 255));
	pHeader->SetButtonTextColor(BUTTON_ITEM_STATUS_NORMAL, Color(255, 255, 255, 255));
	pHeader->AddActionEventListener(*this);
	pHeader->SetTitleTextColor(Color(255, 255, 255, 255));

	Color *textNormalColor = new (std::nothrow) Color(255, 255, 255, 255);
	Color *textSelectedColor = new (std::nothrow) Color(0, 0, 0, 255);
	Color *buttonNormalColor = new (std::nothrow) Color(52, 87, 119, 255);
	Color *buttonSelectedColor = new (std::nothrow) Color(255, 255, 255, 255);

	__pAddButton = new (std::nothrow) Button();
	__pAddButton->Construct(Rectangle(rect.width/2 - buttonWidth/2, yOffset, buttonWidth, 80));
	__pAddButton->SetText(L"Добавить в друзья");
	__pAddButton->SetActionId(1);
	__pAddButton->AddActionEventListener(*this);
	__pAddButton->SetTextColor(*textNormalColor);
	__pAddButton->SetHighlightedTextColor(*textSelectedColor);
	__pAddButton->SetColor(BUTTON_STATUS_NORMAL, *buttonNormalColor);
	__pAddButton->SetColor(BUTTON_STATUS_PRESSED, *buttonSelectedColor);

	AddControl(__pAddButton);

//	__pRemoveButton = new (std::nothrow) Button();
//	__pRemoveButton->Construct(Rectangle(rect.width/2 - buttonWidth/2, rect.height - 80, buttonWidth, 80));
//	__pRemoveButton->SetText(L"Удалить из друзей");
//	__pRemoveButton->SetActionId(2);
//	__pRemoveButton->AddActionEventListener(*this);
//	__pRemoveButton->SetTextColor(*textNormalColor);
//	__pRemoveButton->SetHighlightedTextColor(*textSelectedColor);
//	__pRemoveButton->SetColor(BUTTON_STATUS_NORMAL, *buttonNormalColor);
//	__pRemoveButton->SetColor(BUTTON_STATUS_PRESSED, *buttonSelectedColor);

//	AddControl(__pRemoveButton);

	Image image;
	image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"empty_bg.png";
	Bitmap *pBackgroundBitmap = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);


	Color *labelTextColor = new (std::nothrow) Color(86, 86, 93, 255);

	__pAddLabel = new (std::nothrow) TextBox();
	__pAddLabel->Construct(Rectangle(rect.width/2 - buttonWidth/2, yOffset, buttonWidth, 80), TEXT_BOX_BORDER_NONE);
	__pAddLabel->SetText(L"Заявка отправлена!");
	__pAddLabel->SetTextAlignment(ALIGNMENT_CENTER);
	__pAddLabel->SetTextColor(TEXT_BOX_TEXT_COLOR_NORMAL, *labelTextColor);
	__pAddLabel->SetTextSize(36);
	__pAddLabel->SetFocusable(false);
	__pAddLabel->SetShowState(false);

	__pAddLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_NORMAL, *pBackgroundBitmap);
	__pAddLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_HIGHLIGHTED, *pBackgroundBitmap);
	__pAddLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_DISABLED, *pBackgroundBitmap);

	AddControl(__pAddLabel);

	delete pBackgroundBitmap;
	delete labelTextColor;


	return E_SUCCESS;
}

result
ProfileForm::OnTerminating(void) {

	return E_SUCCESS;
}

void
ProfileForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
ProfileForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs->GetCount() > 0) {
		MUser *user = static_cast< MUser* > (pArgs->GetAt(0));
		this->__pUser = user;
		this->UpdateScreenForUser();
	}
}

void
ProfileForm::UpdateScreenForUser() {
	String *fullName = new String();
	fullName->Append(this->__pUser->GetFirstName()->GetPointer());
	fullName->Append(L" ");
	fullName->Append(this->__pUser->GetLastName()->GetPointer());

	Header *pHeader = this->GetHeader();
	pHeader->SetTitleText(fullName->GetPointer());

//	if (this->__pUser->__isFriend == 1) {
//		__pRemoveButton->SetShowState(true);
//	} else {
//		__pRemoveButton->SetShowState(false);
//	}

	ImageCache::getInstance().LoadImageForTarget(this->__pUser->__pBigPhoto, this);
}

void
ProfileForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
ProfileForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == 1) {
		HashMap *params = new HashMap();
		params->Construct();

		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		String uid;
		uid.Format(10, L"%d", this->__pUser->GetUid());

		params->Add(new String(L"user_id"), new String(uid));

		if (!__pAddToFriendsOperation) {
			__pAddToFriendsOperation = new RestRequestOperation(ADD_TO_FRIEND_REQUEST, new String(L"friends.add"), params);
			__pAddToFriendsOperation->AddEventListener(this);
			RMessageSendDescriptor *descriptor = new RMessageSendDescriptor();
			__pAddToFriendsOperation->SetResponseDescriptor(descriptor);
			RestClient::getInstance().PerformOperation(__pAddToFriendsOperation);
		}
	}
}

void
ProfileForm::OnSuccessN(RestResponse *result) {

	if (result->GetOperationCode() == ADD_TO_FRIEND_REQUEST) {
		if (__pAddToFriendsOperation) {
			__pAddToFriendsOperation->AddEventListener(null);
			__pAddToFriendsOperation = null;
		}

		RMessageSendResponse *response = static_cast<RMessageSendResponse *>(result);

		LinkedList *pArgs = new LinkedList();

		pArgs->Add(new Integer(response->__mid));

		this->SendUserEvent(ADD_TO_FRIEND_REQUEST, pArgs);
		Tizen::App::App::GetInstance()->SendUserEvent(ADD_TO_FRIEND_REQUEST, 0);
	}

}
void
ProfileForm::OnErrorN(Error *error) {
	if (__pAddToFriendsOperation) {
		__pAddToFriendsOperation->AddEventListener(null);
		__pAddToFriendsOperation = null;
	}
}

void
ProfileForm::OnImageLoadedN(Bitmap *result, Integer *code) {
	if (result) {
		__pUserAvatar = result;

		this->SendUserEvent(0, 0);
		Tizen::App::App::GetInstance()->SendUserEvent(0, 0);
	}
}

void
ProfileForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	this->RequestRedraw();

	if (requestId == ADD_TO_FRIEND_REQUEST) {
		if (pArgs->GetCount() > 0) {
			Integer *code = static_cast<Integer *>(pArgs->GetAt(0));

			if (code->ToInt() == 1) {
				this->__pAddButton->SetShowState(false);
				this->__pAddLabel->SetShowState(true);
				this->__pAddLabel->SetText(L"Заявка отправлена!");
			} else if (code->ToInt() == 2) {
				this->__pAddButton->SetShowState(false);
				this->__pAddLabel->SetShowState(true);
				this->__pAddLabel->SetText(L"Заявка одобрена!");
			} else if (code->ToInt() == 4) {
				this->__pAddButton->SetShowState(false);
				this->__pAddLabel->SetShowState(true);
				this->__pAddLabel->SetText(L"Заявка уже была отправлена");
			}
		}
	}
}


result
ProfileForm::OnDraw() {
	result r = E_SUCCESS;

	Canvas* pCanvas = GetCanvasN();

	if (pCanvas != null && __pUserAvatar != null)
	{
		Rectangle bounds = GetClientAreaBounds();

		Rectangle rect = Rectangle(bounds.width/2 - imageSize/2, imageOffset * 2, imageSize, imageSize);
		r = pCanvas->DrawBitmap(rect, *__pUserAvatar);
	}

	delete pCanvas;

	return r;
}

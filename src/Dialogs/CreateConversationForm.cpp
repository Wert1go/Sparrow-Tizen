/*
 * CreateConversationForm.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#include "CreateConversationForm.h"


#include <FBase.h>
#include <FApp.h>
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
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

CreateConversationForm::CreateConversationForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);
	__pChatCreateOperation = null;
	this->__pDialog = new MDialog();
}

CreateConversationForm::~CreateConversationForm() {
	if (__pChatCreateOperation) {
		__pChatCreateOperation->AddEventListener(null);
	}
}

result
CreateConversationForm::OnInitializing(void) {

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"CreateConversationForm");

	clientRect = this->GetClientAreaBounds();

	Header *pHeader = this->GetHeader();
	pHeader->SetColor(Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_NORMAL, Color(65, 97, 137, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_PRESSED, Color(33, 63, 99, 255));
	pHeader->SetButtonColor(BUTTON_ITEM_STATUS_HIGHLIGHTED, Color(33, 63, 99, 255));
	pHeader->SetButtonTextColor(BUTTON_ITEM_STATUS_NORMAL, Color(255, 255, 255, 255));
	pHeader->AddActionEventListener(*this);
	pHeader->SetTitleTextColor(Color(255, 255, 255, 255));
	String titleText(L"Беседа");

	pHeader->SetTitleText(titleText);

	String cancelText(L"Готово");

	ButtonItem *cancelButton = new ButtonItem();
	cancelButton->Construct(BUTTON_ITEM_STYLE_TEXT, 23);
	cancelButton->SetText(cancelText);
	pHeader->SetButton(BUTTON_POSITION_RIGHT, *cancelButton);

	__pEditArea = new EditArea();
	__pEditArea->Construct(
				Rectangle(10, 10, clientRect.width - 20, 100 - 20),
				INPUT_STYLE_OVERLAY
		);

	Panel *panel = new Panel();
	panel->Construct(Rectangle(0, 0, clientRect.width, 100));
	panel->SetBackgroundColor(Color(23, 30, 38, 255));
	AddControl(panel);

	String guideText(L"Название беседы");

	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_NORMAL, Color(255, 255, 255, 255));
	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_HIGHLIGHTED, Color(255, 255, 255, 255));
	__pEditArea->SetTextColor(EDIT_TEXT_COLOR_LINK, Color(255, 255, 255, 255));
	__pEditArea->SetTextSize(42);
	__pEditArea->SetGuideText(guideText);
	__pEditArea->SetColor(EDIT_STATUS_NORMAL, Color(0,0,0,255));
	__pEditArea->SetColor(EDIT_STATUS_HIGHLIGHTED, Color(0,0,0,255));
	__pEditArea->SetColor(EDIT_STATUS_PRESSED, Color(0,0,0,255));

	AddControl(__pEditArea);

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - 100), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8,8, 255));
	Color *separatorColor = new (std::nothrow) Color(48, 48, 48, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(1, L"Test1");
	__pItemContext->AddElement(2, L"Test2");
	AppLogDebug("OnInitia!!!!2321lizing");
	return r;
}

result
CreateConversationForm::OnTerminating(void) {
	return E_SUCCESS;
}

void
CreateConversationForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	AppAssert(pArgs);

	this->__pUsersList = new LinkedList();

	for (int i = 0; i < pArgs->GetCount(); i++) {
		this->__pUsersList->Add(pArgs->GetAt(i));
	}

	AppLogDebug("CONVERSATION %d", this->__pUsersList->GetCount());

	if (this->__pListView) {
		this->__pListView->UpdateList();
	}
}

void
CreateConversationForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
CreateConversationForm::RequestUiUpdate(int code, IList *pArgs) {
	this->SendUserEvent(code, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(code, 0);
}

void
CreateConversationForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs) {

	if (5555555 == requestId) {
		AppLog("OnUserEventReceivedN");
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);

		ArrayList *paramsList = new (std::nothrow) ArrayList();
		paramsList->Construct();


		paramsList->Add(new Integer(this->__pDialog->GetUid()));
		pSceneManager->GoForward(
				ForwardSceneTransition(
						SCENE_CHAT,
						SCENE_TRANSITION_ANIMATION_TYPE_LEFT,
						SCENE_HISTORY_OPTION_NO_HISTORY,
						SCENE_DESTROY_OPTION_DESTROY), paramsList);
	} else if (requestId == 111111) {

		AppAssert(pArgs->GetCount() > 0);
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));

		__pListView->RefreshList(unit->__index, unit->__requestId);
	} else {

		try {

			this->__pListView->UpdateList();
		} catch (...) {

		}
	}

	delete pArgs;
}

void
CreateConversationForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {

	CreateChat();
}

void
CreateConversationForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}


void
CreateConversationForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state) {

}


void
CreateConversationForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status) {

}
void
CreateConversationForm::OnListViewItemSwept(ListView &listView, int index, SweepDirection direction)
{

}


ListItemBase*
CreateConversationForm::CreateItem(int index, int itemWidth)
{
	AppLogDebug("AppLogDebug(OnInitializing);");
	UiDialogCustomItem* pItem = new UiDialogCustomItem();
	ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

	int height = 136;

	pItem->Construct(Dimension(itemWidth, height), style);

	float annexWidth = pItem->GetAnnexWidth(style);

	pItem->SetDimension(new Dimension(itemWidth - annexWidth, height));
	pItem->SetIndex(index);

	pItem->AddRefreshListener(this);


	MUser *dialog = static_cast<MUser *>(__pUsersList->GetAt(index));

	pItem->SetUser(dialog);

	pItem->Init();

    return pItem;
}

bool
CreateConversationForm::DeleteItem(int index, ListItemBase* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

int
CreateConversationForm::GetItemCount(void)
{
	if (__pUsersList) {
		return __pUsersList->GetCount();
	} else {
		return 0;
	}
}

void
CreateConversationForm::RequestUpdateForIndex(int index, int elementId) {
	ArrayList *list = new ArrayList();
	list->Construct(1);

	UpdateUnit *updateUnit = new UpdateUnit();

	updateUnit->__index = index;

	updateUnit->__requestId = elementId;

	list->Add(updateUnit);


	this->SendUserEvent(111111, list);
	Tizen::App::App::GetInstance()->SendUserEvent(111111, 0);
}

void
CreateConversationForm::RequestImageUpdateForIndex(int index, int section, int elementId) {

}

//REST

void
CreateConversationForm::OnSuccessN(RestResponse *result) {
	if (__pChatCreateOperation) {
		__pChatCreateOperation->AddEventListener(null);
		__pChatCreateOperation = null;
	}

	if (result->GetOperationCode() == CREATE_CHAT) {

		RMessageSendResponse* response = static_cast<RMessageSendResponse *>(result);
		AppLog("CREATE_CHAT %d", response->__mid);
		this->__pDialog->SetChatId(response->__mid);
		this->__pDialog->SetUid(response->__mid + isChatValue);
		MDialogDao::getInstance().Save(this->__pDialog);
		AppLog("SAVED! %d", response->__mid);
		this->SendUserEvent(5555555, 0);
		Tizen::App::App::GetInstance()->SendUserEvent(5555555, 0);
	}

}

void
CreateConversationForm::OnErrorN(Error *error) {
	if (__pChatCreateOperation) {
		__pChatCreateOperation->AddEventListener(null);
		__pChatCreateOperation = null;
	}

}

void
CreateConversationForm::CreateChat() {
	HashMap *params = new HashMap();
	params->Construct();

	String *chatTitle = new String(this->__pEditArea->GetText());

	String *uids = new String(L"");

	for (int i = 0; i < this->__pUsersList->GetCount(); i++) {
		MUser *user = static_cast<MUser *>(this->__pUsersList->GetAt(i));

		String uid;
		uid.Format(10, L"%d", user->GetUid());

		uids->Append(uid);

		if (i != this->__pUsersList->GetCount() - 1) {
			uids->Append(L",");
		}
	}

	if (uids->GetLength() > 0) {
		params->Add(new String(L"user_ids"), uids);
	}
	params->Add(new String(L"title"), chatTitle);

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	this->__pDialog->SetTitle(new String(chatTitle->GetPointer()));
	this->__pDialog->SetChatUids(uids);


	if (!__pChatCreateOperation) {
		__pChatCreateOperation = new RestRequestOperation(CREATE_CHAT, new String(L"messages.createChat"), params);
		__pChatCreateOperation->AddEventListener(this);
		__pChatCreateOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__pChatCreateOperation);
	}

}

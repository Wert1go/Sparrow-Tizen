/*
 * UiMessagesPanel.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#include <FApp.h>

#include "UiMessagesPanel.h"
#include "UiDialogCustomItem.h"
#include "UpdateUnit.h"
#include "MDialogDao.h"
#include "MUserDao.h"

#include "RDialogResponse.h"
#include "MDialogsDescriptor.h"
#include "Error.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "AuthManager.h"
#include "LongPollConnection.h"

#include "UiUpdateConstants.h"
#include "SceneRegister.h"
#include "MainForm.h"
#include "Resources.h"

#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

UiMessagesPanel::UiMessagesPanel() {
	AppLogDebug("UiMessagesPanel");

	__pDialogsList = null;
	__pDialogRequestOperation = null;
	this->__pListView = null;
}

UiMessagesPanel::~UiMessagesPanel() {
	if (__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
	}
}

bool
UiMessagesPanel::Initialize(void)
{
	result r = Construct(Rectangle(0, 0, 10, 10));	// Should be set proper area at OnInitializing().
	TryReturn(!IsFailed(r), false, "%s", GetErrorMessage(r));
	return true;
}

result
UiMessagesPanel::OnInitializing(void)
{

	AppLogDebug("OnInitializing");
	result r = E_SUCCESS;
	Rectangle clientRect;
	this->SetName(L"UiMessagesPanel");

	const Form* pForm = dynamic_cast<Form*>(GetParent());
	AppAssert(pForm);
	clientRect = pForm->GetClientAreaBounds();
	SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height));

	Panel *panel = new Panel();
	panel->Construct(Rectangle(0, 0, clientRect.width, 100));
	panel->SetBackgroundColor(Color(23, 30, 38, 255));
	AddControl(panel);

	Button *addButton = new Button();

	addButton->Construct(Rectangle(
			clientRect.width - 100 + 70/5,
			100/2 - 70/2,
			70,
			70));
	addButton->SetActionId(103);
	addButton->AddActionEventListener(*this);
	addButton->SetColor(BUTTON_STATUS_NORMAL, Color(44, 51, 59, 255));
	addButton->SetColor(BUTTON_STATUS_PRESSED, Color(44, 51, 59, 255));

	Bitmap *image = Resources::getInstance().LoadBitmapNamed(L"edit_icon.png");
	addButton->SetNormalBitmap(FloatPoint(70/2 - 54/2, 70/2 - 54/2), *image);
	addButton->SetPressedBitmap(FloatPoint(70/2 - 54/2, 70/2 - 54/2), *image);
	delete image;

	AddControl(addButton);

	__pSearchBar = new SearchBar();
	__pSearchBar->Construct(Rectangle(0, 0, clientRect.width - 100, 100));
	__pSearchBar->SetGuideText(L"Поиск");
	__pSearchBar->AddSearchBarEventListener(*this);
	__pSearchBar->AddTextEventListener(*this);
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_NORMAL, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_DISABLED, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_NORMAL, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_DISABLED, Color(115, 120, 145, 255));
	__pSearchBar->SetColor(Color(23, 30, 38, 255));

	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_NORMAL, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pSearchBar->SetButtonTextColor(SEARCH_BAR_BUTTON_STATUS_DISABLED, Color(115, 120, 145, 255));

	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_NORMAL, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_HIGHLIGHTED, Color(0, 0, 0, 255));
	__pSearchBar->SetButtonColor(SEARCH_BAR_BUTTON_STATUS_PRESSED, Color(0, 0, 0, 255));

	AddControl(__pSearchBar);

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - 100), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);
	__pListView->AddScrollEventListener(*this);
	Color *separatorColor = new (std::nothrow) Color(48, 48, 48, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	__pListView->SetSweepEnabled(false);
	delete separatorColor;

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(ID_CONTEXT_ITEM_1, L"Test1");
	__pItemContext->AddElement(ID_CONTEXT_ITEM_2, L"Test2");

	return r;
}

result
UiMessagesPanel::OnTerminating() {
	result r = E_SUCCESS;
	if (__pDialogRequestOperation) {
			__pDialogRequestOperation->AddEventListener(null);
	}
	return r;
}

void
UiMessagesPanel::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
	if (this->__pListView) {
		this->__pListView->UpdateList();
	}
	SendRequest();

	UpdateUnreadCount();
}

void
UiMessagesPanel::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

	if (__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}

	this->__pSearchBar->HideKeypad();
	 Form* pForm = dynamic_cast<Form*>(GetParent());
	pForm->Invalidate(true);
	AppLog("+++++++++++++++++++++ HEIGHT: %d",pForm->GetClientAreaBounds().height);
}

void
UiMessagesPanel::OnListViewItemStateChanged(ListView &listView, int index, int elementId, ListItemStatus status)
{
	if (status != LIST_ITEM_STATUS_SELECTED) {
		return;
	}

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ArrayList *paramsList = new (std::nothrow) ArrayList();
	paramsList->Construct();

	MDialog *dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));

	paramsList->Add(new Integer(dialog->GetUid()));
	pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
}

void
UiMessagesPanel::OnListViewContextItemStateChanged(ListView &listView, int index, int elementId, ListContextItemStatus state)
{

}

void
UiMessagesPanel::OnListViewItemSwept(ListView &listView, int index, SweepDirection direction)
{

}

// IListViewItemProvider implementation
ListItemBase*
UiMessagesPanel::CreateItem(int index, int itemWidth)
{
//	AppLog("CreateItem :: %d", index);
	UiDialogCustomItem *pItem = new UiDialogCustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

    int height = 136;

    pItem->Construct(Dimension(itemWidth, height), style);
//    pItem->SetContextItem(__pItemContext);
    pItem->SetDimension(new Dimension(itemWidth, height));
    pItem->SetIndex(index);
    pItem->AddRefreshListener(this);

    MDialog *dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));
    pItem->SetDialog(dialog);

    pItem->Init();
    return pItem;
}

bool
UiMessagesPanel::DeleteItem(int index, ListItemBase* pItem, int itemWidth)
{
    delete pItem;
    pItem = null;
    return true;
}

int
UiMessagesPanel::GetItemCount(void)
{
	if (this->GetDialogsList()) {
		return this->GetDialogsList()->GetCount();
	} else {
		return 0;
	}
}

void
UiMessagesPanel::RequestUpdateForIndex(int index, int elementId) {
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
UiMessagesPanel::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {

//	AppLogDebug("OnUserEventReceivedN");
	if (requestId == 111111) {
		AppAssert(pArgs->GetCount() > 0);
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));
		__pListView->RefreshList(unit->__index, unit->__requestId);
		delete unit;

	} else if (requestId == 222222) {
		__pListView->UpdateList();
	} else if (requestId == UPDATE_USER_ONLINE) {

		AppAssert(pArgs->GetCount() > 0);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		this->UpdateItemListWithUserId(userId->ToInt(), 1);
		delete userId;

	} else if (requestId == UPDATE_USER_OFFLINE) {

		AppAssert(pArgs->GetCount() > 0);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		this->UpdateItemListWithUserId(userId->ToInt(), 0);

		delete userId;
	} else if (requestId == UPDATE_MESSAGE_ARRIVED) {
		this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
		this->__pListView->UpdateList();
		UpdateUnreadCount();
	} else if (requestId == UPDATE_READ_STATE) {
		AppAssert(pArgs->GetCount() > 0);
		Integer *msgId = static_cast<Integer*>(pArgs->GetAt(0));
		this->SetReadStateWithMessageId(msgId->ToInt());
		UpdateUnreadCount();
		delete msgId;
	}

	delete pArgs;
}

void
UiMessagesPanel::UpdateItemListWithUserId(int userId, int value) {
	int indexToUpdate = -1;

	for (int index = 0; index < this->GetDialogsList()->GetCount(); index++) {
		MDialog *dialog = static_cast<MDialog*>(this->GetDialogsList()->GetAt(index));
		if (dialog->GetUid() == userId) {
			dialog->SetIsOnline(value);
			indexToUpdate = index;
			break;
		}
	}

	if (indexToUpdate >= 0 && indexToUpdate < this->GetDialogsList()->GetCount()) {
		this->__pListView->RefreshList(indexToUpdate, 23);
	}
}

void
UiMessagesPanel::SetReadStateWithMessageId(int msgId) {
	int indexToUpdate = -1;

	for (int index = 0; index < this->GetDialogsList()->GetCount(); index++) {
		MDialog *dialog = static_cast<MDialog*>(this->GetDialogsList()->GetAt(index));

//		AppLog("%d %d", dialog->GetIdentifier(), msgId);

		if (dialog->GetIdentifier() == msgId) {
			dialog->SetReadState(1);
			indexToUpdate = index;
			break;
		}
	}

	if (indexToUpdate >= 0) {
		this->__pListView->RefreshList(indexToUpdate, 45);
		this->__pListView->RefreshList(indexToUpdate, 23);
	}
}

// ************************** DATA ***************************//

void
UiMessagesPanel::SetDialogsList(LinkedList *list) {

	if (this->__pDialogsList) {
		delete this->__pDialogsList;
		__pDialogsList = null;
	}

	__pDialogsList = list;
}

LinkedList *
UiMessagesPanel::GetDialogsList() {
	if (!__pDialogsList) {
		__pDialogsList = new LinkedList();
	}

	return __pDialogsList;
}

void
UiMessagesPanel::OnSuccessN(RestResponse *result) {
	if(__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}

	RDialogResponse *response = static_cast<RDialogResponse *>(result);

	this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
	this->SendUserEvent(222222, 0);
	Tizen::App::App::GetInstance()->SendUserEvent(222222, 0);
}

void
UiMessagesPanel::OnErrorN(Error *error) {
	if(__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}
}

void
UiMessagesPanel::SendRequest(int offset) {

	HashMap *params = new HashMap();
	params->Construct();

	String offsetString;
	offsetString.Format(10, L"%d", offset);

	String requestBegin = L"var a = API.messages.getDialogs({\"offset\" : ";

	requestBegin.Append(offsetString);

	String requestEnd = L", \"count\":20});"
			"var l = a.items@.user_id;"
			"var c = a.items@.chat_id;"
			"var i = 0;"
			"var uids = [];"
			"var j;"
			"while (i < c.length) {"
			"	i=i+1;"
			" 	if (parseInt(c[i]) != 0) {"
			"		j = API.messages.getChatUsers({\"chat_id\" : c[i]}); "
			"		uids = uids + [{\"chat_id\" : c[i], \"uids\" : j}]; "
			"		l = l + j;"
			" 	}"
			"};"
			"var b = API.users.get({\"user_ids\": l, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200\"});"
			"return {\"chat_uids\" : uids, \"users\": b, \"messages\": a};";

	String request(L"");
	request.Append(requestBegin);
	request.Append(requestEnd);

	params->Add(new String(L"code"), new String(request));

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pDialogRequestOperation) {
		__pDialogRequestOperation = new RestRequestOperation(GET_DIALOGS_EXECUTE, new String(L"execute"), params);
		__pDialogRequestOperation->AddEventListener(this);
		__pDialogRequestOperation->SetResponseDescriptor(new MDialogsDescriptor());
		RestClient::getInstance().PerformOperation(__pDialogRequestOperation);
	}
}

void
UiMessagesPanel::UpdateUnreadCount() {
	MainForm *form = static_cast<MainForm *>(this->GetParent());
	form->UpdateUnreadCount();
}

void
UiMessagesPanel::RequestImageUpdateForIndex(int index, int section, int elementId) {

}

void
UiMessagesPanel::OnTextValueChanged(const Tizen::Ui::Control& source) {

	String string = this->__pSearchBar->GetText();

	if (string.GetLength() == 0) {
//		AppLog("OnTextValueChanged!!!!!");
//		this->__pUsersList = MUserDao::getInstance().GetFriendsN();
	} else {
//		AppLog("OnTextValueChanged %S", string.GetPointer());
//
//		this->__pUsersList = MUserDao::getInstance().SearchUsers(new String(string.GetPointer()));
//
//		AppLog("RESULTS %d", this->__pUsersList->GetCount());
	}

//	this->__pListView->UpdateList();
}

void
UiMessagesPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == 103) {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_NEW_MESSAGE, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), 0);
	}
}

void
UiMessagesPanel::OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type) {
	if (type == SCROLL_END_EVENT_END_BOTTOM) {
		if (this->__pDialogsList && this->__pDialogsList->GetCount() > 0) {
			this->SendRequest(this->__pDialogsList->GetCount());
		}
	}
}

result
UiMessagesPanel::OnDraw() {

	String importantString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH, importantString);
	__pSearchBar->SetGuideText(importantString);

	return E_SUCCESS;
}

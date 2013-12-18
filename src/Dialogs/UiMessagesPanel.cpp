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
#include "UiUpdateConstants.h"
#include "RSearchDialogDescriptor.h"

#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

static String searchTextHolder;

UiMessagesPanel::UiMessagesPanel() {
	AppLogDebug("UiMessagesPanel");

	__pDialogsList = null;
	__pSearchDialogsList = null;
	__pSearchMessagesList = null;

	__pDialogRequestOperation = null;

	__pSearchDialogRequestOperation = null;
	__pSearchMessageRequestOperation = null;

	__pListUpdateTimer = null;
	this->__pListView = null;
	__isSearchMode = false;
	__searchModeCode = SEARCH_DIALOG_MODE;
	__pAddButton = null;
}

UiMessagesPanel::~UiMessagesPanel() {
	if (__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
	}

	if (__pSearchDialogRequestOperation) {
		__pSearchDialogRequestOperation->AddEventListener(null);
	}

	if (__pSearchMessageRequestOperation) {
		__pSearchMessageRequestOperation->AddEventListener(null);
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
	__pAddButton = addButton;
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

	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_NORMAL, Color(1, 1, 1, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(0, 0, 0, 255));
	__pSearchBar->SetSearchFieldColor(SEARCH_FIELD_STATUS_DISABLED, Color(0, 0, 0, 255));

	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_NORMAL, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_HIGHLIGHTED, Color(115, 120, 145, 255));
	__pSearchBar->SetSearchFieldTextColor(SEARCH_FIELD_STATUS_DISABLED, Color(115, 120, 145, 255));
	__pSearchBar->SetColor(Color(23, 30, 38, 255));
	__pSearchBar->AddActionEventListener(*this);

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

	if (this->__pListUpdateTimer) {
		this->__pListUpdateTimer->Cancel();
		delete this->__pListUpdateTimer;
		this->__pListUpdateTimer = null;
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
UiMessagesPanel::ShowNewMessageButton() {
	Rectangle clientRect;
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	clientRect = pForm->GetClientAreaBounds();

	__pSearchBar->SetBounds(Rectangle(0, 0, clientRect.width - 100, 100));
	__pAddButton->SetBounds(Rectangle(
				clientRect.width - 100 + 70/5,
				100/2 - 70/2,
				70,
				70));
	this->Invalidate(true);
}

void
UiMessagesPanel::HideNewMessageButton() {
	Rectangle clientRect;
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	clientRect = pForm->GetClientAreaBounds();

	__pSearchBar->SetBounds(Rectangle(0, 0, clientRect.width, 100));
	__pAddButton->SetBounds(Rectangle(0,0,0,0));
	this->Invalidate(true);
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


	if (this->SearchModeIsActive()) {
		if (this->__searchModeCode == SEARCH_DIALOG_MODE) {
			AppLog("-----------------------------------");
			MDialog *dialog = static_cast<MDialog *>(this->GetSearchDialogsList()->GetAt(index));
			if (dialog->GetUsers()) {
				AppLog("dialog: %d", dialog->GetUsers()->GetCount());
			}
			paramsList->Add(new Integer(dialog->GetUid()));
			paramsList->Add(dialog);
		} else {
			MDialog *dialog = static_cast<MDialog *>(this->GetSearchMessagesList()->GetAt(index));
			paramsList->Add(new Integer(dialog->GetUid()));
			paramsList->Add(dialog);
		}
	} else {
		MDialog *dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));
		paramsList->Add(new Integer(dialog->GetUid()));
	}

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
    pItem->SetDimension(new Dimension(itemWidth, height));
    pItem->SetIndex(index);
    pItem->AddRefreshListener(this);

    MDialog *dialog = null;

    if (this->SearchModeIsActive()) {
    	if (this->__searchModeCode == SEARCH_DIALOG_MODE) {
    		AppLog("Create");
    		dialog = static_cast<MDialog *>(this->GetSearchDialogsList()->GetAt(index));
    		AppLog("Create 1");
    		pItem->SetUser(dialog->GetUser());
    		AppLog("Create 2");

    	} else {
    		dialog = static_cast<MDialog *>(this->GetSearchMessagesList()->GetAt(index));
    		pItem->SetDialog(dialog);
    	}
    } else {
    	dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));
    	pItem->SetDialog(dialog);
    }



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
UiMessagesPanel::GetItemCount(void) {
	if (this->SearchModeIsActive()) {
		if (this->__searchModeCode == SEARCH_DIALOG_MODE) {
			return this->GetSearchDialogsList()->GetCount();
		} else {
			return this->GetSearchMessagesList()->GetCount();
		}
	} else {
		if (this->GetDialogsList()) {
			return this->GetDialogsList()->GetCount();
		} else {
			return 0;
		}
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
		if (!this->SearchModeIsActive()) {
			__pListView->RefreshList(unit->__index, unit->__requestId);
		}
		delete unit;

	} else if (requestId == 222222) {
		if (!this->SearchModeIsActive()) {
			__pListView->UpdateList();
		}
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
		if (!this->SearchModeIsActive()) {
			this->__pListView->UpdateList();
		}
		UpdateUnreadCount();
	} else if (requestId == UPDATE_READ_STATE) {
		AppAssert(pArgs->GetCount() > 0);
		Integer *msgId = static_cast<Integer*>(pArgs->GetAt(0));
		this->SetReadStateWithMessageId(msgId->ToInt());
		UpdateUnreadCount();
		delete msgId;
	} else if (requestId == UPDATE_SEARCH_MESSAGES || requestId == UPDATE_SEARCH_DIALOGS) {
		if (this->SearchModeIsActive()) {
			if (requestId == UPDATE_SEARCH_DIALOGS) {
				LinkedList *list = static_cast<LinkedList *>(pArgs->GetAt(0));
				AppLog("list: %d", list->GetCount());
				this->SetSearchDialogsList(list);
				AppLog("+list: %d", list->GetCount());
			}

			this->__pListView->UpdateList();
		}
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

	if (!this->SearchModeIsActive()) {
		if (indexToUpdate >= 0 && indexToUpdate < this->GetDialogsList()->GetCount()) {
			this->__pListView->RefreshList(indexToUpdate, 23);
		}
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

	if (!this->SearchModeIsActive()) {
		if (indexToUpdate >= 0) {
			this->__pListView->RefreshList(indexToUpdate, 45);
			this->__pListView->RefreshList(indexToUpdate, 23);
		}
	}
}

// ************************** DATA ***************************//

void
UiMessagesPanel::SetDialogsList(LinkedList *list) {

	if (this->__pDialogsList) {
		__pDialogsList->RemoveAll(true);
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
UiMessagesPanel::SetSearchDialogsList(LinkedList *list){

	if (this->__pSearchDialogsList) {
		__pSearchDialogsList->RemoveAll(true);
		delete this->__pSearchDialogsList;
		__pSearchDialogsList = null;
	}

	__pSearchDialogsList = list;
}

LinkedList *
UiMessagesPanel::GetSearchDialogsList() {
	if (!__pSearchDialogsList) {
		__pSearchDialogsList = new LinkedList();
	}

	return __pSearchDialogsList;
}

void
UiMessagesPanel::SetSearchMessagesList(LinkedList *list){

	if (this->__pSearchMessagesList) {
		__pSearchMessagesList->RemoveAll(true);
		delete this->__pSearchMessagesList;
		__pSearchMessagesList = null;
	}

	__pSearchMessagesList = list;
}

LinkedList *
UiMessagesPanel::GetSearchMessagesList() {
	if (!__pSearchMessagesList) {
		__pSearchMessagesList = new LinkedList();
	}

	return __pSearchMessagesList;
}

void
UiMessagesPanel::OnSuccessN(RestResponse *result) {

	if (result->GetOperationCode() == GET_DIALOGS_EXECUTE) {
		if(__pDialogRequestOperation) {
			__pDialogRequestOperation->AddEventListener(null);
			__pDialogRequestOperation = null;
		}

		this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
		this->SendUserEvent(222222, 0);
		Tizen::App::App::GetInstance()->SendUserEvent(222222, 0);

	} else if (result->GetOperationCode() == SEARCH_MESSAGES) {
		if (__pSearchMessageRequestOperation) {
			__pSearchMessageRequestOperation->AddEventListener(null);
			__pSearchMessageRequestOperation = null;
		}

		RDialogResponse *response = static_cast<RDialogResponse *>(result);

		if (this->SearchModeIsActive() && this->__searchModeCode == SEARCH_MESSAGE_MODE) {
			this->SetSearchMessagesList(response->GetDialogs());
			this->SendUserEvent(UPDATE_SEARCH_MESSAGES, 0);
			Tizen::App::App::GetInstance()->SendUserEvent(UPDATE_SEARCH_MESSAGES, 0);
		}

	} else if (result->GetOperationCode() == SEARCH_DIALOGS) {
		if (__pSearchDialogRequestOperation) {
			__pSearchDialogRequestOperation->AddEventListener(null);
			__pSearchDialogRequestOperation = null;
		}

		RDialogResponse *response = static_cast<RDialogResponse *>(result);

		if (this->SearchModeIsActive() && this->__searchModeCode == SEARCH_DIALOG_MODE) {
			LinkedList *params = new LinkedList();
			params->Add(response->GetDialogs());
//			this->SetSearchDialogsList(response->GetDialogs());
			this->SendUserEvent(UPDATE_SEARCH_DIALOGS, params);
			Tizen::App::App::GetInstance()->SendUserEvent(UPDATE_SEARCH_DIALOGS, 0);
		}
	}
}

void
UiMessagesPanel::OnErrorN(Error *error) {
	if(__pDialogRequestOperation) {
		__pDialogRequestOperation->AddEventListener(null);
		__pDialogRequestOperation = null;
	}
	if (__pSearchMessageRequestOperation) {
		__pSearchMessageRequestOperation->AddEventListener(null);
		__pSearchMessageRequestOperation = null;
	}
	if (__pSearchDialogRequestOperation) {
		__pSearchDialogRequestOperation->AddEventListener(null);
		__pSearchDialogRequestOperation = null;
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
			" 	if (parseInt(c[i]) != 0) {"
			"		j = API.messages.getChatUsers({\"chat_id\" : c[i]}); "
			"		uids = uids + [{\"chat_id\" : c[i], \"uids\" : j}]; "
			"		l = l + j;"
			" 	}"
			"	i=i+1;"
			"};"
			"var b = API.users.get({\"user_ids\": l, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200,last_seen\"});"
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
		this->SetSearchMode(false);
	} else {
		if (!this->SearchModeIsActive()) {
			this->SetSearchMode(true);
		}

		searchTextHolder = string;
		if (this->__pListUpdateTimer) {
			this->__pListUpdateTimer->Cancel();
			delete this->__pListUpdateTimer;
			this->__pListUpdateTimer = null;
		}

		this->__pListUpdateTimer = new Timer();
		this->__pListUpdateTimer->Construct(*this);
		this->__pListUpdateTimer->Start(400);
	}

}

void
UiMessagesPanel::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == 103) {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_NEW_MESSAGE, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), 0);
	}

	if (actionId == 0) {
		if (this->__pListUpdateTimer) {
			this->__pListUpdateTimer->Cancel();
			delete this->__pListUpdateTimer;
			this->__pListUpdateTimer = null;
		}

		searchTextHolder = NULL;
		this->SetSearchMode(false);

		AppLog("__________________________________");
	}

	AppLog("-------------++++++++++++++++ %d", actionId);
}

void
UiMessagesPanel::OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type) {
	if (type == SCROLL_END_EVENT_END_BOTTOM) {
		if (this->__pDialogsList && this->__pDialogsList->GetCount() > 0) {
			this->SendRequest(this->__pDialogsList->GetCount());
		}
	}
}

/****************************** SEARCH ***************************/

void
UiMessagesPanel::SetSearchMode(bool state) {
	__isSearchMode = state;
	MainForm *form = static_cast<MainForm *>(this->GetParent());
	form->SetSearchMode(state);

	if (state) {
		AppLog("rtet rt rter ");
		this->SetSearchModeCode(SEARCH_DIALOG_MODE);
		this->HideNewMessageButton();
	} else {
		this->ShowNewMessageButton();
		searchTextHolder = NULL;
		if (this->__pListUpdateTimer) {
			this->__pListUpdateTimer->Cancel();
			delete this->__pListUpdateTimer;
			this->__pListUpdateTimer = null;
		}

		this->__pListView->UpdateList();
	}
}

bool
UiMessagesPanel::SearchModeIsActive() {
	return __isSearchMode;
}

void
UiMessagesPanel::SetSearchModeCode(int code) {
	AppLog("SetSearchModeCode: %d", code);

	__searchModeCode = code;

	String string = this->__pSearchBar->GetText();

	if (string.GetLength() != 0) {
		if (this->__searchModeCode == SEARCH_MESSAGE_MODE) {
			this->SearchMessages(string);
		} else {
			this->SearchDialogs(string);
		}
	}

	this->__pListView->UpdateList();
}

void
UiMessagesPanel::SearchDialogs(String searchText) {
	HashMap *params = new HashMap();
	params->Construct();

	String requestBegin = L"var a = API.messages.searchDialogs({ \"q\" : \"";
	requestBegin.Append(searchText);
	requestBegin.Append(L"\", ");
	requestBegin.Append(L"\"fields\": \"photo_100,photo_50,online,is_friend,photo_200\" });");
	String requestEnd = L""
			"var u = a@.users;"
			"var r = \"\";"
			"var i = 0;"
			"while (i < u.length) {"
			"	var s = u[i] + \"\";"
			"	if (s != \"\") {"
			"		r = r + u[i];"
			"		if (i != u.length - 1) {"
			"			r = r + \",\";"
			"		}"
			"	}"
			"	i = i + 1;"
			"};"
			"var b = API.users.get({\"user_ids\": r, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200,last_seen\"});"
			"return {\"dialogs\" : a, \"users\": b};";

	String request(L"");
	request.Append(requestBegin);
	request.Append(requestEnd);

	params->Add(new String(L"code"), new String(request));

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pSearchDialogRequestOperation) {
		__pSearchDialogRequestOperation = new RestRequestOperation(SEARCH_DIALOGS, new String(L"execute"), params);
		__pSearchDialogRequestOperation->AddEventListener(this);
		__pSearchDialogRequestOperation->SetResponseDescriptor(new RSearchDialogDescriptor());
		RestClient::getInstance().PerformOperation(__pSearchDialogRequestOperation);
	}
}

void
UiMessagesPanel::SearchMessages(String searchText) {
	AppLog("__pSearchMessageRequestOperation");

	HashMap *params = new HashMap();
	params->Construct();

	AppLog("__pSearchMessageRequestOperation");

	int offset = 0;

	String offsetString;
	offsetString.Format(10, L"%d", offset);

	String requestBegin = L"var a = API.messages.search({\"offset\" : ";

	requestBegin.Append(offsetString);
	requestBegin.Append(L", \"q\" : \"");
	requestBegin.Append(searchText);
	requestBegin.Append(L"\", ");
	String requestEnd = L"\"count\":20});"
			"var l = a.items@.user_id;"
			"var c = a.items@.chat_id;"
			"var i = 0;"
			"var uids = [];"
			"var j;"
			"while (i < c.length) {"
			" 	if (parseInt(c[i]) != 0) {"
			"		j = API.messages.getChatUsers({\"chat_id\" : c[i]}); "
			"		uids = uids + [{\"chat_id\" : c[i], \"uids\" : j}]; "
			"		l = l + j;"
			" 	}"
			"	i=i+1;"
			"};"
			"var b = API.users.get({\"user_ids\": l, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200,last_seen\"});"
			"return {\"chat_uids\" : uids, \"users\": b, \"messages\": a};";

	String request(L"");
	request.Append(requestBegin);
	request.Append(requestEnd);

	params->Add(new String(L"code"), new String(request));

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pSearchMessageRequestOperation) {
		__pSearchMessageRequestOperation = new RestRequestOperation(SEARCH_MESSAGES, new String(L"execute"), params);
		__pSearchMessageRequestOperation->AddEventListener(this);
		__pSearchMessageRequestOperation->SetResponseDescriptor(new MDialogsDescriptor(false));
		RestClient::getInstance().PerformOperation(__pSearchMessageRequestOperation);
	}
}

void
UiMessagesPanel::OnTimerExpired (Timer &timer) {
	if (this->__pListUpdateTimer) {
		if (searchTextHolder != NULL) {
			AppLog("2OnTimerExpired %S", searchTextHolder.GetPointer());
			if (searchTextHolder.GetLength() == 0) {

			} else {
				if (this->__searchModeCode == SEARCH_MESSAGE_MODE) {
					this->SearchMessages(searchTextHolder);
				} else {
					this->SearchDialogs(searchTextHolder);
				}
			}
		}

		delete this->__pListUpdateTimer;
		this->__pListUpdateTimer = null;
	}
}

result
UiMessagesPanel::OnDraw() {

	String importantString;
	Application::GetInstance()->GetAppResource()->GetString(IDS_SEARCH, importantString);
	__pSearchBar->SetGuideText(importantString);

	return E_SUCCESS;
}

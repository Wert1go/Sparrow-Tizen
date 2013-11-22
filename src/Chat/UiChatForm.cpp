/*
 * UiChatForm.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "UiChatForm.h"

#include "SceneRegister.h"
#include "MMessageDescriptor.h"
#include "RMessageSendDescriptor.h"
#include "RMessageSendResponse.h"
#include "RestClient.h"
#include "AuthManager.h"
#include "RMessagesResponse.h"
#include "MMessage.h"
#include "MMessageDao.h"

#include "UiUpdateConstants.h"

#include <FGraphics.h>
#include <FMedia.h>
#include "UiChatCustomItem.h"
#include "PostMan.h"
#include "UiChapPanel.h"
#include "MUserDao.h"
#include "MUser.h"
#include "MDialogDao.h"
#include "Util.h"

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

UiChatForm::UiChatForm() {
	result r = E_SUCCESS;

	r = Form::Construct(FORM_STYLE_NORMAL);

	SetFormBackEventListener(this);
	__pMessagesRequestOperation = null;
	__pMarkAsReadRequestOperation = null;
	__pMessages = null;
	__pListView = null;
}

UiChatForm::~UiChatForm() {
	if (__pMessagesRequestOperation) {
		__pMessagesRequestOperation->AddEventListener(null);
		__pMessagesRequestOperation = null;
	}

	if (__pMarkAsReadRequestOperation) {
		__pMarkAsReadRequestOperation->AddEventListener(null);
		__pMarkAsReadRequestOperation = null;
	}
}



result
UiChatForm::OnInitializing(void)
{

	AppLog("OnInitializing");
	this->SetName(L"UiChatForm");
	result r = E_SUCCESS;
	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();

	__pPosterPanel = new UiMessengerPanel();
	__pPosterPanel->Initialize();
	__pPosterPanel->SetBackgroundColor(Color(23, 30, 38, 255));

	__pPosterPanel->GetEditArea()->AddKeypadEventListener(*this);
	__pPosterPanel->GetEditArea()->AddTextEventListener(*this);
	__pPosterPanel->GetEditArea()->AddExpandableEditAreaEventListener(*this);

	__pPosterPanel->GetSendButton()->AddActionEventListener(*this);
	AppLog("0");
	float editAreaHeight = 100;
	//__pPosterPanel->SetBounds(FloatRectangle(0, clientRect.height - editAreaHeight,  clientRect.width, editAreaHeight));
	__pPosterPanel->SetRectangle(FloatRectangle(0, clientRect.height - editAreaHeight,  clientRect.width, editAreaHeight));

	AddControl(__pPosterPanel);

	__pChatPanel = new UiChapPanel();
	__pChatPanel->Initialize();
	__pChatPanel->SetBounds(Rectangle(0, 0, clientRect.width, 100));
	AddControl(__pChatPanel);

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - editAreaHeight *2), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);
	__pListView->AddScrollEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8, 8, 255));
	__pListView->SetItemDividerColor(Color(0, 0, 0, 0));

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(ID_CONTEXT_ITEM_1, L"Test1");
	__pItemContext->AddElement(ID_CONTEXT_ITEM_2, L"Test2");

	return r;
}

result
UiChatForm::OnTerminating() {
	result r = E_SUCCESS;

	PostMan::getInstance().RemoveListenerForUser(__userId);

	return r;
}

void
UiChatForm::SetMessages(LinkedList *messages) {
	if (__pMessages) {
		delete __pMessages;
	}
	__pMessages = messages;
}

LinkedList *
UiChatForm::GetMessages() {
	return __pMessages;
}

/********************** SCENE MANAGMENT ***********************/

void
UiChatForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {

	AppLogDebug("+++++++++++++++++++++++++++++++++++++++++++");

	SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
		AppLogDebug("1111+++++++++++++++++++++++++++++++++++++++++++");
}


void
UiChatForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs->GetCount() > 0) {
		Integer *param = static_cast< Integer* > (pArgs->GetAt(0));
		__userId = param->ToInt();
		this->SetMessages(MMessageDao::getInstance().GetMessagesForUser(param->ToInt()));
		this->ScrollToFirstMessage();
		RequestMessagesForUser(param->ToInt());
		if (this->__pChatPanel) {
			MDialog *dialog = MDialogDao::getInstance().GetDialogN(__userId);

			if (!dialog) {
				MUser *user = MUserDao::getInstance().GetUserN(__userId);
				dialog = MDialog::CreateFromUserN(user);
			}

			this->__pDialog = dialog;

			this->__pChatPanel->SetDialog(dialog);
		}
		MarkUnread();
	}

}

void
UiChatForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	if (__pMessagesRequestOperation) {
		__pMessagesRequestOperation->AddEventListener(null);
		__pMessagesRequestOperation = null;
	}
}

/*
 * загрузка последних 20 сообщений из бд
 * если нет с сервера
 *
 * долистали до вершины списка:
 * загрузка еще 20 сообщений из БД
 * если нет с сервера
 *
 */

/********************** NET REQUESTS ***********************/

void
UiChatForm::RequestMessagesForUser(int userId) {
	if (!__pMessagesRequestOperation) {
	}

	this->__userId = userId;

	HashMap *params = new HashMap();
	String uidString;
	uidString.Format(25, L"%d", userId);

	params->Construct();
	params->Add(new String(L"user_id"), new String(uidString));
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pMessagesRequestOperation) {
		__pMessagesRequestOperation = new RestRequestOperation(GET_MESSAGES_HISTORY, new String(L"messages.getHistory"), params);
		__pMessagesRequestOperation->AddEventListener(this);
		__pMessagesRequestOperation->SetResponseDescriptor(new MMessageDescriptor());
		RestClient::getInstance().PerformOperation(__pMessagesRequestOperation);
	}
}

void
UiChatForm::RequestMoreMessagesFromMid(int mid) {
	HashMap *params = new HashMap();
	String uidString;
	uidString.Format(25, L"%d", __userId);
	String midString;
	midString.Format(25, L"%d", mid);

	params->Construct();
	params->Add(new String(L"user_id"), new String(uidString));
	params->Add(new String(L"rev"), new String(L"1"));
	params->Add(new String(L"start_message_id"), new String(midString));
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pMessagesRequestOperation) {
		__pMessagesRequestOperation = new RestRequestOperation(GET_MESSAGES_HISTORY_BACKWARD, new String(L"messages.getHistory"), params);
		__pMessagesRequestOperation->AddEventListener(this);
		__pMessagesRequestOperation->SetResponseDescriptor(new MMessageDescriptor());
		RestClient::getInstance().PerformOperation(__pMessagesRequestOperation);
	}
}

/************************* LIST VIEW ************************/

void
UiChatForm::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state) {

}

void
UiChatForm::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status) {

}

void
UiChatForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView &listView, int index, Tizen::Ui::Controls::SweepDirection direction) {

}

		// IListViewItemProvider

Tizen::Ui::Controls::ListItemBase*
UiChatForm::CreateItem(int index, int itemWidth) {

	UiChatCustomItem *pItem = new UiChatCustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;
    MMessage *message = static_cast<MMessage *>(this->GetMessages()->GetAt(index));
    Dimension dmns = Util::CalculateDimensionForMessage(message);
    dmns.height = dmns.height +  20*4;
    dmns.width = dmns.width +  20*4;

    int height = dmns.height;
    if (height < 136) {
    	height = 136;
    	dmns.height = height;
    }

    pItem->Construct(Dimension(itemWidth, height), style);
    pItem->SetContextItem(__pItemContext);

    pItem->SetBubbleDimension(dmns);
    pItem->SetMessage(message);

    pItem->SetDimension(new Dimension(itemWidth, height));
    pItem->SetDialog(this->__pDialog);
//    pItem->SetIndex(index);
//    pItem->AddRefreshListener(this);

    pItem->Init();

    return pItem;
}

bool
UiChatForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth) {
	return true;
}

int
UiChatForm::GetItemCount(void) {
	if (this->GetMessages()) {
		return this->GetMessages()->GetCount();
	} else {
		return 0;
	}
}


/********************** NET CALLBACK ***********************/

void
UiChatForm::OnSuccessN(RestResponse *result) {


	RMessagesResponse *response = static_cast<RMessagesResponse *>(result);

	if (result->GetOperationCode() == GET_MESSAGES_HISTORY_BACKWARD) {
		if(__pMessagesRequestOperation) {
			__pMessagesRequestOperation->AddEventListener(null);
			__pMessagesRequestOperation = null;
		}

		LinkedList *messages = new LinkedList();
		if (response->GetMessages()->GetCount() > 0) {
			//ВК возвращает 1 повторяющееся сообщение
			response->GetMessages()->RemoveAt(response->GetMessages()->GetCount() - 1);
		}
		messages->AddItems(* response->GetMessages());
		messages->AddItems(*this->GetMessages());

		this->SetMessages(messages);
	} else if (MARK_AS_READ == result->GetOperationCode()) {
		if(this->__pMarkAsReadRequestOperation) {
			__pMarkAsReadRequestOperation->AddEventListener(null);
			__pMarkAsReadRequestOperation = null;
		}

		RMessageSendResponse *response = static_cast<RMessageSendResponse *>(result);
		if (response->GetError()) {
			AppLogDebug("FAILED MARK AS UNREAD");
		} else {
			if (response->__mid == 1) {
				MMessageDao::getInstance().markAsReaded(__userId, 0);
			}
		}

	} else {
		this->SetMessages(MMessageDao::getInstance().GetMessagesForUser(this->__userId));
	}

	this->SendUserEvent(result->GetOperationCode(), 0);
	Tizen::App::App::GetInstance()->SendUserEvent(result->GetOperationCode(), 0);
}

void
UiChatForm::OnErrorN(Error *error) {

}

void
UiChatForm::OnMessageDelivered(int userId, MMessage *message) {
	LinkedList *pArgs = new LinkedList();

	pArgs->Add(new Integer(userId));
	pArgs->Add(message);

	this->SendUserEvent(UPDATE_MESSAGE_DELIVERED, pArgs);
	Tizen::App::App::GetInstance()->SendUserEvent(UPDATE_MESSAGE_DELIVERED, 0);
}

/********************** THREADS DISPATCH ***********************/

void
UiChatForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == GET_MESSAGES_HISTORY_BACKWARD && __pListView) {
		this->__pListView->UpdateList();
		ScrollToLastMessage();
	} else if (requestId == GET_MESSAGES_HISTORY) {
		this->__pListView->UpdateList();
		this->ScrollToFirstMessage();
	} else if (requestId == UPDATE_MESSAGE_ARRIVED || requestId == UPDATE_MESSAGE_DELIVERED) {
		AppAssert(pArgs && pArgs->GetCount() == 2);
		Integer *pUserId = static_cast<Integer *>(pArgs->GetAt(0));

		int userId = pUserId->ToInt();

		if (userId == __userId) {
			MMessage *pMessage = static_cast<MMessage *>(pArgs->GetAt(1));

			if (!this->IsAlreadyAdded(pMessage)) {
				this->GetMessages()->Add(pMessage);
				this->__pListView->RefreshList(this->GetMessages()->GetCount() - 1, LIST_REFRESH_TYPE_ITEM_ADD);
				this->ScrollToFirstMessage();
			}
		}

		this->MarkUnread();
		delete pUserId;
	} else if (requestId == UPDATE_USER_ONLINE) {

		AppAssert(pArgs->GetCount() > 0);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		if (this->__userId == userId->ToInt()) {
			this->UpdateCurrentUserOnlineWithValue(1);
		}

		delete userId;

	} else if (requestId == UPDATE_USER_OFFLINE) {

		AppAssert(pArgs->GetCount() > 0);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		if (this->__userId == userId->ToInt()) {
			this->UpdateCurrentUserOnlineWithValue(0);
		}
		delete userId;
	} else if (requestId == UPDATE_READ_STATE) {
		AppAssert(pArgs->GetCount() > 0);
		Integer *msgId = static_cast<Integer*>(pArgs->GetAt(0));

		this->SetReadStateWithMessageId(msgId->ToInt());
		delete msgId;
	}

	delete pArgs;
}

void
UiChatForm::UpdateCurrentUserOnlineWithValue(int value) {
	if (this->__pChatPanel) {
		this->__pChatPanel->SetIsOnline(value == 1);
	}
}

void
UiChatForm::SetReadStateWithMessageId(int msgId) {
	int indexToUpdate = -1;
	AppLogDebug("MMessage %d", msgId);
	for (int index = 0; index < this->GetMessages()->GetCount(); index++) {
		MMessage *message = static_cast<MMessage*>(this->GetMessages()->GetAt(index));
		if (message->GetMid() == msgId) {
			message->SetReadState(1);
			indexToUpdate = index;
			break;
		}
	}

	if (indexToUpdate >= 0) {
		this->__pListView->RefreshList(indexToUpdate, 23);
	}
}

bool
UiChatForm::IsAlreadyAdded(MMessage *message) {
	//Огромное допущение вероятности!!1
	bool result = false;

	int limit = 0;
	if (this->GetMessages()->GetCount() > 32) {
		limit = this->GetMessages()->GetCount() - 30;
	}

	for (int index = this->GetMessages()->GetCount()-1; index > limit; index--) {
		MMessage *existingMessage = static_cast<MMessage*>(this->GetMessages()->GetAt(index));
		if (existingMessage->GetMid() == message->GetMid()) {
			result = true;
			break;
		}
	}

	return result;
}

/********************** EDIT CALLBACK ***********************/

void
UiChatForm::OnTextValueChanged(const Tizen::Ui::Control& source) {

}

void
UiChatForm::OnTextValueChangeCanceled(const Tizen::Ui::Control& source) {

}

void
UiChatForm::OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount) {

	FloatRectangle editBounds = source.GetBoundsF();

	FloatRectangle clientArea = this->GetClientAreaBoundsF();

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();
	float prevEditHeight = panelBounds.height - 32;

	float deltaHeight = editBounds.height - prevEditHeight;

	this->__pListView->SetBounds(FloatRectangle(
			0,
			100,
			clientArea.width,
			this->__pListView->GetBounds().height - deltaHeight)
			);
AppLog("1");
	__pPosterPanel->SetRectangle(FloatRectangle(
			panelBounds.x,
			100 + this->__pListView->GetBounds().height,
			panelBounds.width,
			panelBounds.height + deltaHeight)
			);

	this->Invalidate(true);
}

void
UiChatForm::OnExpandableEditAreaLineRemoved(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount) {
	FloatRectangle editBounds = source.GetBoundsF();

	FloatRectangle clientArea = this->GetClientAreaBoundsF();

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();
	float prevEditHeight = panelBounds.height - 32;

	float deltaHeight = prevEditHeight - editBounds.height;
	AppLog("2 %f", deltaHeight);

	if (deltaHeight < 1) {
		return;
	}

	this->__pListView->SetBounds(FloatRectangle(
			0,
			100,
			clientArea.width,
			this->__pListView->GetBoundsF().height + deltaHeight)
			);

	__pPosterPanel->SetRectangle(FloatRectangle(
			panelBounds.x,
			100 + this->__pListView->GetBoundsF().height,
			panelBounds.width,
			panelBounds.height - deltaHeight)
			);

	this->Invalidate(true);
}

void
UiChatForm::OnKeypadActionPerformed(Control& source, KeypadAction keypadAction)
{
}

void
UiChatForm::OnKeypadClosed(Control& source)
{

	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();
	FloatRectangle editBounds = source.GetBoundsF();
	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();

	this->__pPosterPanel->SetRectangle(FloatRectangle(0, clientRect.height - panelBounds.height, clientRect.width, panelBounds.height));
	this->__pListView->SetBounds(FloatRectangle(0, 100, clientRect.width, clientRect.height - panelBounds.height - 100));
	this->Invalidate(true);

	this->SetFocus();
}

void
UiChatForm::OnKeypadOpened(Control& source)
{

}

void
UiChatForm::OnKeypadWillOpen(Control& source)
{
	FloatRectangle editBounds = source.GetBoundsF();
	FloatRectangle prevBounds = source.GetParent()->GetBoundsF();

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();

	float size = 34.6875;
	float yOffset = this->GetBoundsF().height - (this->GetBoundsF().height * size)/100;

	AppLogDebug("yOffset %f %f", yOffset - panelBounds.height, yOffset);

	this->__pListView->SetBounds(FloatRectangle(0, 100, prevBounds.width, yOffset - panelBounds.height - 100));
	__pPosterPanel->SetRectangle(FloatRectangle(panelBounds.x, yOffset - panelBounds.height, panelBounds.width, panelBounds.height));

	this->Invalidate(true);
}

void
UiChatForm::OnKeypadBoundsChanged(Control& source)
{

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();

	FloatRectangle editBounds = source.GetBoundsF();
	AppLog("keypad %f, %f, %f, %f", editBounds.x, editBounds.y, editBounds.width, editBounds.height);
}

/********************** BUTTON CALLBACK ***********************/

void
UiChatForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == 45) {
		if (__pPosterPanel->GetEditArea()->GetText().GetLength() != 0) {
			SendMessage();
		}
		__pPosterPanel->GetEditArea()->SetText(L"");
		__pPosterPanel->GetEditArea()->HideKeypad();
	}
}

/********************** SEND MESSAGE ********************/

void
UiChatForm::SendMessage() {
	String *messageText = new String(__pPosterPanel->GetEditArea()->GetText());

	MMessage *pMessage = new MMessage();

	pMessage->SetUid(__userId);
	pMessage->SetText(messageText);
	pMessage->SetOut(1);
	pMessage->SetReadState(0);
	pMessage->SetDelivered(0);

	PostMan::getInstance().SendMessageFromUserWithListener(pMessage, __userId, this);

}

/********************** SCROLL ***********************/

void
UiChatForm::OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type) {
	if (type == SCROLL_END_EVENT_END_TOP) {
		AppLogDebug("TOP REACHED");
		MMessage *firstMessage = static_cast<MMessage *>(this->GetMessages()->GetAt(0));

		LinkedList *fetchedMessages = MMessageDao::getInstance().GetMessagesForUser(__userId, firstMessage->GetMid());

		this->__lastMessageId = firstMessage->GetMid();

		if (fetchedMessages->GetCount() == 0) {
			RequestMoreMessagesFromMid(firstMessage->GetMid());
		} else {
			LinkedList *messages = new LinkedList();
			messages->AddItems(*fetchedMessages);
			messages->AddItems(*this->GetMessages());

			this->SetMessages(messages);
			this->__pListView->UpdateList();
			ScrollToLastMessage();
		}
	}
}

void
UiChatForm::ScrollToLastMessage() {

	MMessage* pMessage = null;

	for (int index = 0; index < this->GetMessages()->GetCount(); index++) {
		pMessage = static_cast<MMessage *>(this->GetMessages()->GetAt(index));

		if (pMessage->GetMid() == this->__lastMessageId) {
			this->__pListView->ScrollToItem(index);
			break;
		}
	}
}

void
UiChatForm::ScrollToFirstMessage() {
	if (this->GetMessages() && this->GetMessages()->GetCount() > 0) {
		this->__pListView->ScrollToItem(this->GetMessages()->GetCount() - 1);
	}
}

void
UiChatForm::MarkUnread() {
	int id = MMessageDao::getInstance().firstUnreadMessage(__userId, 0);

	if (id > 0) {
		AppLogDebug("MarkUnread %d", id);

		HashMap *params = new HashMap();
		String uidString;
		uidString.Format(25, L"%d", __userId);
		String midString;
		midString.Format(25, L"%d", id);

		params->Construct();
		params->Add(new String(L"user_id"), new String(uidString));
		params->Add(new String(L"start_message_id"), new String(midString));
		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		if (!__pMarkAsReadRequestOperation) {
			__pMarkAsReadRequestOperation = new RestRequestOperation(MARK_AS_READ, new String(L"messages.markAsRead"), params);
			__pMarkAsReadRequestOperation->AddEventListener(this);
			__pMarkAsReadRequestOperation->SetResponseDescriptor(new RMessageSendDescriptor());
			RestClient::getInstance().PerformOperation(__pMarkAsReadRequestOperation);
		}
	}
}

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
#include "MAttachment.h"
#include "MGeo.h"
#include "Util.h"
#include "UpdateUnit.h"
#include "UiAttachmentListPopup.h"

#include <iostream>
#include <sys/time.h>

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

const int itemContentSize = 180;

UiChatForm::UiChatForm() {
	result r = E_SUCCESS;

	r = Form::Construct(FORM_STYLE_NORMAL);

	SetFormBackEventListener(this);
	__pMessagesRequestOperation = null;
	__pMarkAsReadRequestOperation = null;
	__pNotifyUserPrintingOperation = null;

	__pMessages = new LinkedList();
	__pListView = null;

	__pPrintingTimer = null;
	__isUserPrinting = false;
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

	this->SetBackgroundColor(Color(8, 8, 8, 255));

	__pPosterPanel = new UiMessengerPanel();
	__pPosterPanel->Initialize();
	__pPosterPanel->SetBackgroundColor(Color(23, 30, 38, 255));

	__pPosterPanel->GetEditArea()->AddKeypadEventListener(*this);
	__pPosterPanel->GetEditArea()->AddTextEventListener(*this);
	__pPosterPanel->GetEditArea()->AddExpandableEditAreaEventListener(*this);

	__pPosterPanel->GetSendButton()->AddActionEventListener(*this);
	__pPosterPanel->GetAttachButton()->AddActionEventListener(*this);
	__pPosterPanel->__pAttachmentOwner = this;
	float editAreaHeight = 100;
	__pPosterPanel->SetRectangle(FloatRectangle(0, clientRect.height - editAreaHeight,  clientRect.width, editAreaHeight));

	AddControl(__pPosterPanel);

	__pChatPanel = new UiChapPanel();
	__pChatPanel->Initialize();
	__pChatPanel->SetBounds(Rectangle(0, 0, clientRect.width, 100));
	AddControl(__pChatPanel);

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 100, clientRect.width, clientRect.height - editAreaHeight - 100), true, SCROLL_STYLE_FADE_OUT);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);
	__pListView->AddScrollEventListener(*this);
	__pListView->SetBackgroundColor(Color(8, 8, 8, 255));
	__pListView->SetItemDividerColor(Color(0, 0, 0, 0));
	__pListView->SetSweepEnabled(false);
	__pListView->SetShowState(false);

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(ID_CONTEXT_ITEM_1, L"Test1");
	__pItemContext->AddElement(ID_CONTEXT_ITEM_2, L"Test2");
	this->__isActive = true;

	__pAttachmentPopup = new UiAttachmentListPopup();
	__pAttachmentPopup->Construct(true, FloatDimension(630.0f, 750.0f));
	this->__pAttachmentPopup->__pPopupHandler = this;

	return r;
}

result
UiChatForm::OnTerminating() {
	result r = E_SUCCESS;

	PostMan::getInstance().RemoveListenerForUser(__userId);
	__pPosterPanel->__pAttachmentOwner = null;
	__pAttachmentPopup->Destroy();

	if (this->__pPrintingTimer) {
		this->__pPrintingTimer->Cancel();
		delete this->__pPrintingTimer;
		this->__pPrintingTimer = null;
	}

	if (__pMessages) {
		__pMessages->RemoveAll(true);
		delete __pMessages;
		__pMessages = null;
	}

	return r;
}

void
UiChatForm::SetMessages(LinkedList *messages) {
	if (__pMessages) {
		__pMessages->RemoveAll();
		delete __pMessages;
	}
	__pMessages = messages;
}

LinkedList *
UiChatForm::GetMessages() {
	if (!__pMessages) {
		__pMessages = new LinkedList();
	}
	return __pMessages;
}

/********************** SCENE MANAGMENT ***********************/

void
UiChatForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	this->__isActive = false;;

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}


void
UiChatForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {
	AppLog("OnSceneActivatedN");

	if (pArgs && pArgs->GetCount() > 0) {
		Integer *param = static_cast< Integer* > (pArgs->GetAt(0));

		__userId = param->ToInt();

		if (this->__pChatPanel) {
			MDialog *dialog = MDialogDao::getInstance().GetDialogN(__userId);

			if (!dialog) {
				if (pArgs->GetCount() > 1) {
					dialog = static_cast< MDialog* > (pArgs->GetAt(1));
				} else {
					MUser *user = MUserDao::getInstance().GetUserN(__userId);
					dialog = MDialog::CreateFromUserN(user);
				}
			}

			this->__pDialog = dialog;

			this->__pChatPanel->SetDialog(dialog);
		}

		this->SetMessages(MMessageDao::getInstance().GetMessagesForUser(param->ToInt()));
		this->__pListView->UpdateList();
		this->ScrollToFirstMessage();

		if (this->__pListView && this->GetMessages()->GetCount() > 0) {
			this->__pListView->SetShowState(true);
			this->Invalidate(true);
		}

		RequestMessagesForUser(param->ToInt());

		MarkUnread();
//
		PostMan::getInstance().__pAttachmentListener = this;
//
		RestoreAttachmentContainer();

		delete pArgs;
	}

}

void
UiChatForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	if (__pMessagesRequestOperation) {
		__pMessagesRequestOperation->AddEventListener(null);
		__pMessagesRequestOperation = null;
	}

	PostMan::getInstance().__pAttachmentListener = null;
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

	this->__userId = userId;

	HashMap *params = new HashMap();
	String uidString;
	uidString.Format(25, L"%d", userId);

	params->Construct();

	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	String historyRequest(L"");

	historyRequest.Append(L"var a = API.messages.getHistory({\"user_id\" : ");
	historyRequest.Append(uidString);
	historyRequest.Append(L"});");
	historyRequest.Append(L""
	"var l = a.items@.fwd_messages;"
	"var i = 0;"
	"var uids = [];"
	"var j;"

	"while (i < l.length) {"
		"i = i+ 1;"

		"var fwd = l[i]@.user_id;"
		"uids = uids + fwd;"
	"};"

	"var b = [];"
	"if (uids.length > 0) {"
		"var b = API.users.get({\"user_ids\": uids, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200\"});"
	"}"

	"return {\"users\": b, \"count\" : a.count, \"items\" : a.items};");

	params->Add(new String(L"code"), new String(historyRequest));

	if (!__pMessagesRequestOperation) {
		__pMessagesRequestOperation = new RestRequestOperation(GET_MESSAGES_HISTORY, new String(L"execute"), params);
		__pMessagesRequestOperation->AddEventListener(this);
		__pMessagesRequestOperation->SetResponseDescriptor(new MMessageDescriptor());
		AppLog("PerformOperation");
		RestClient::getInstance().PerformOperation(__pMessagesRequestOperation);
	}
}

void
UiChatForm::RequestMoreMessagesFromMid(int mid) {
	AppLog("RequestMoreMessagesFromMid");

	HashMap *params = new HashMap();
	String uidString;
	uidString.Format(25, L"%d", __userId);
	String midString;
	midString.Format(25, L"%d", mid);

	params->Construct();
//	params->Add(new String(L"user_id"), new String(uidString));
//	params->Add(new String(L"rev"), new String(L"1"));
//	params->Add(new String(L"start_message_id"), new String(midString));
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	String historyRequest(L"");

	historyRequest.Append(L"var a = API.messages.getHistory({\"user_id\" : ");
	historyRequest.Append(uidString);
	historyRequest.Append(L", \"rev\" : 1, ");
	historyRequest.Append(L"\"start_message_id\" :");
	historyRequest.Append(midString);
	historyRequest.Append(L"});");
	historyRequest.Append(L""
	"var l = a.items@.fwd_messages;"
	"var i = 0;"
	"var uids = [];"
	"var j;"

	"while (i < l.length) {"
		"i = i+ 1;"

		"var fwd = l[i]@.user_id;"
		"uids = uids + fwd;"
	"};"

	"var b = [];"
	"if (uids.length > 0) {"
		"var b = API.users.get({\"user_ids\": uids, \"fields\": \"photo_100,photo_50,online,is_friend,photo_200\"});"
	"}"

	"return {\"users\": b, \"count\" : a.count, \"items\" : a.items};");

		params->Add(new String(L"code"), new String(historyRequest));

	if (!__pMessagesRequestOperation) {
		__pMessagesRequestOperation = new RestRequestOperation(GET_MESSAGES_HISTORY_BACKWARD, new String(L"execute"), params);
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

	if (status == LIST_ITEM_STATUS_SELECTED) {
		MMessage *message = static_cast<MMessage *>(this->GetMessages()->GetAt(index));

		AppLog("elementId:: %d", elementId);

		int pointer = 0;


		MAttachment *attach = this->FindAttachment(message, pointer, elementId);

		if (attach) {
			AppLog("attach: %S", attach->__pType->GetPointer());

			if (attach->__pType->Equals(PHOTO, false)) {


				SceneManager* pSceneManager = SceneManager::GetInstance();
				AppAssert(pSceneManager);

				ArrayList *paramsList = new (std::nothrow) ArrayList();
				paramsList->Construct();

				String *imgUrl = null;

				if (attach->__pPhoto604) {
					imgUrl = attach->__pPhoto604;
				} else {
					imgUrl = attach->__pPhoto130;
				}

				paramsList->Add(new String(imgUrl->GetPointer()));
				pSceneManager->GoForward(ForwardSceneTransition(SCENE_IMAGE_VIEWER, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
			} else if (attach->__pType->Equals(VIDEO, false)) {
				SceneManager* pSceneManager = SceneManager::GetInstance();
				AppAssert(pSceneManager);

				ArrayList *paramsList = new (std::nothrow) ArrayList();
				paramsList->Construct();

				paramsList->Add(attach);
				pSceneManager->GoForward(ForwardSceneTransition(SCENE_VIDEO_VIEWER, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
			}
		}
	}
}

MAttachment *
UiChatForm::FindAttachment(MMessage *pMessage, int &pointer, int elementId) {
	MAttachment *attach = null;




	if (pMessage->__pAttachments) {
		for (int i = 0; i < pMessage->__pAttachments->GetCount(); i++, pointer++) {
			if (pointer == elementId) {
				attach = static_cast<MAttachment *>(pMessage->__pAttachments->GetAt(i));
				return attach;
			}
		}
	}

	if (pMessage->__pGeo) {
		if (pointer == elementId) {
			attach = pMessage->__pGeo;
			return attach;
		} else {
			pointer++;
		}
	}

	if (pMessage->__pFwd) {
//		pointer++
		int fwdPointer = pointer;

		for (int j = 0; j < pMessage->__pFwd->GetCount(); j++, fwdPointer++) {
			AppLog("int fwdPointer %d :: elementId %d", fwdPointer, elementId);
			MMessage *pFwdMessage = static_cast<MMessage *>(pMessage->__pFwd->GetAt(j));
			if (fwdPointer == elementId) {
				attach = pFwdMessage;
				break;
			} else {
//				if (j != 0) {
					pointer = fwdPointer + pMessage->__pFwd->GetCount();
//				}
				AppLog("pointer %d", pointer);
				attach = this->FindAttachment(pFwdMessage, pointer, elementId);
				if (attach) {
					break;
				}
			}

		}
	}

	return attach;
}

void
UiChatForm::OnListViewItemSwept(Tizen::Ui::Controls::ListView &listView, int index, Tizen::Ui::Controls::SweepDirection direction) {
	AppLog("tttttt");
}

		// IListViewItemProvider

Tizen::Ui::Controls::ListItemBase*
UiChatForm::CreateItem(int index, int itemWidth) {

	UiChatCustomItem *pItem = new UiChatCustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;
    MMessage *message = static_cast<MMessage *>(this->GetMessages()->GetAt(index));
    Dimension dmns = Util::CalculateDimensionForMessage(message);

    dmns.height = dmns.height +  20*4;
    dmns.width = dmns.width +  20*2;

    int height = dmns.height;
    if (height < 136) {
    	height = 136;
    	dmns.height = height;
    }

    pItem->Construct(Dimension(itemWidth, height), style);

    pItem->SetBubbleDimension(dmns);
    pItem->SetMessage(message);

    pItem->SetDimension(new Dimension(itemWidth, height));
    pItem->SetDialog(this->__pDialog);
    pItem->AddRefreshListener(this);
    pItem->SetIndex(index);

    pItem->Init();

    return pItem;
}

bool
UiChatForm::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth) {
	delete pItem;
	pItem = null;
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
	AppLog("OnSuccessN");
	RMessagesResponse *response = static_cast<RMessagesResponse *>(result);

	bool needUpdate = true;

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
		messages->AddItems(*response->GetMessages());
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

	} else if (NOTIFY_USER_PRINTING == result->GetOperationCode()) {
		__pNotifyUserPrintingOperation->AddEventListener(null);
		__pNotifyUserPrintingOperation = null;
		needUpdate = false;
	} else {
		if (__pMessagesRequestOperation) {
			__pMessagesRequestOperation->AddEventListener(null);
			__pMessagesRequestOperation = null;
		}

		MMessage *serverMessage = null;
		MMessage *localMessage = null;

		if (response->GetMessages()->GetCount() > 0) {
			serverMessage = static_cast<MMessage *>(response->GetMessages()->GetAt(response->GetMessages()->GetCount() - 1));
		}

		if(this->GetMessages() && this->GetMessages()->GetCount() > 0) {
			localMessage = static_cast<MMessage *>(this->GetMessages()->GetAt(0));
		}

		needUpdate = (!localMessage || (serverMessage->GetMid() != localMessage->GetMid()));

		if (needUpdate) {
			this->SetMessages(MMessageDao::getInstance().GetMessagesForUser(this->__userId));
		}
	}

	if (needUpdate) {
		this->SendUserEvent(result->GetOperationCode(), 0);
		Tizen::App::App::GetInstance()->SendUserEvent(result->GetOperationCode(), 0);
	}
}

void
UiChatForm::OnErrorN(Error *error) {
	if (__pMessagesRequestOperation) {
		__pMessagesRequestOperation = null;
	}
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

	if (!__isActive) {
		return;
	}

	if (requestId == 111111) {
		AppAssert(pArgs->GetCount() > 0);
		UpdateUnit *unit = static_cast<UpdateUnit *> (pArgs->GetAt(0));
		__pListView->RefreshList(unit->__index, unit->__requestId);
		delete unit;
	} else if (requestId == GET_MESSAGES_HISTORY_BACKWARD && __pListView) {
		this->__pListView->UpdateList();
		ScrollToLastMessage();
	} else if (requestId == GET_MESSAGES_HISTORY) {
		this->__pListView->UpdateList();
		this->ScrollToFirstMessage();

		if (!this->__pListView->IsVisible() && this->GetMessages()->GetCount() > 0) {
			this->__pListView->SetShowState(true);
			this->Invalidate(true);
		}

		MarkUnread();
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

				if (!this->__pListView->IsVisible() && this->GetMessages()->GetCount() > 0) {
					this->__pListView->SetShowState(true);
					this->Invalidate(true);
				}
			}
		}

		this->MarkUnread();
		delete pUserId;

		if (requestId == UPDATE_MESSAGE_DELIVERED) {
			__pPosterPanel->GetEditArea()->HideKeypad();

			if (__pPosterPanel->__pItems->GetCount() > 0) {
				__pPosterPanel->ClearAttachments();
				this->ResetAttachmentsContainer();
			}

			__pPosterPanel->GetEditArea()->Clear();

			this->__pPosterPanel->GetEditArea()->SetEnabled(true);
			this->__pPosterPanel->GetSendButton()->SetEnabled(true);
			this->__pPosterPanel->GetAttachButton()->SetEnabled(true);
		}

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
	} else if (requestId == UPDATE_USER_PRINTING) {
		AppAssert(pArgs->GetCount() > 0);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		if (this->__userId == userId->ToInt()) {
			this->__pChatPanel->SetUserPrinting(0);
		}
		delete userId;

	} else if (requestId == UPDATE_USER_PRINTING_IN_CONVERSATION) {
		AppAssert(pArgs->GetCount() > 1);
		Integer *userId = static_cast<Integer*>(pArgs->GetAt(0));
		Integer *chatId = static_cast<Integer*>(pArgs->GetAt(1));

		if (this->__userId == (chatId->ToInt() + isChatValue)) {
			this->__pChatPanel->SetUserPrinting(userId->ToInt());
		}

		delete userId;
		delete chatId;
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

	for (int index = 0; index < this->GetMessages()->GetCount(); index++) {
		MMessage *message = static_cast<MMessage*>(this->GetMessages()->GetAt(index));
		if (message->GetMid() == msgId) {
			message->SetReadState(1);
			indexToUpdate = index;
			break;
		}
	}

//	AppLogDebug("MMessage %d :: indexToUpdate %d", msgId, indexToUpdate);

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

	for (int index = this->GetMessages()->GetCount()-1; index >= limit; index--) {
		AppLog("-----------------------");
		MMessage *existingMessage = static_cast<MMessage*>(this->GetMessages()->GetAt(index));

		if (existingMessage->GetMid() == message->GetMid()) {
			AppLog("----------------------- %d %d", existingMessage->GetMid(), message->GetMid());
			result = true;
			break;
		}
	}

	return result;
}

/********************** EDIT CALLBACK ***********************/

void
UiChatForm::OnTextValueChanged(const Tizen::Ui::Control& source) {
	if (!__isUserPrinting) {
		this->NotifyUserTyping();
	}
}

void
UiChatForm::OnTextValueChangeCanceled(const Tizen::Ui::Control& source) {

}

void
UiChatForm::OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount) {
	FloatRectangle editBounds = source.GetBoundsF();

	FloatRectangle clientArea = this->GetClientAreaBoundsF();

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();
	int addedHeight = 0;

	if (this->__pPosterPanel->__pItems->GetCount() > 0) {
		addedHeight = itemContentSize;
	}

	float prevEditHeight = panelBounds.height - editAreaOffset*2 - addedHeight;

	float deltaHeight = editBounds.height - prevEditHeight;
	if (deltaHeight < 10) {
		return;
	}

	this->__pListView->SetBounds(FloatRectangle(
			0,
			100,
			clientArea.width,
			this->__pListView->GetBounds().height - deltaHeight)
			);
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

	int addedHeight = 0;

	if (this->__pPosterPanel->__pItems->GetCount() > 0) {
		addedHeight = itemContentSize;
	}

	float prevEditHeight = panelBounds.height - editAreaOffset*2 - addedHeight;


	float deltaHeight = prevEditHeight - editBounds.height;

	if (deltaHeight < 10) {
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

	if (keypadAction == KEYPAD_ACTION_ENTER) {

	}

}

void
UiChatForm::OnKeypadClosed(Control& source)
{

	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();
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
//	int index = this->__pListView->GetItemIndexFromPosition(100, this->__pListView->GetBounds().y + this->__pListView->GetBounds().height - 50);

//	AppLog("INDEX: %d", index);

	FloatRectangle editBounds = source.GetBoundsF();
	FloatRectangle prevBounds = source.GetParent()->GetBoundsF();

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();

	float size = 34.6875;
	float yOffset = this->GetBoundsF().height - (this->GetBoundsF().height * size)/100;

	this->__pListView->SetBounds(FloatRectangle(0, 100, prevBounds.width, yOffset - panelBounds.height - 100));
	__pPosterPanel->SetRectangle(FloatRectangle(panelBounds.x, yOffset - panelBounds.height, panelBounds.width, panelBounds.height));

	this->Invalidate(true);

	this->ScrollToFirstMessage();
}

void
UiChatForm::OnKeypadBoundsChanged(Control& source)
{

	FloatRectangle panelBounds = __pPosterPanel->GetBoundsF();

	FloatRectangle editBounds = source.GetBoundsF();
}

/********************** BUTTON CALLBACK ***********************/

void
UiChatForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {
	if (actionId == 45) {

		if ((__pPosterPanel->GetEditArea()->GetText().GetLength() != 0 && !__pPosterPanel->GetEditArea()->GetText().Equals(L"", true)) ||
			PostMan::getInstance().GetAttachmentsForUid(__userId)->GetCount() > 0
		) {
			this->SendMessage();

			this->__pPosterPanel->GetEditArea()->SetEnabled(false);
			this->__pPosterPanel->GetSendButton()->SetEnabled(false);
			this->__pPosterPanel->GetAttachButton()->SetEnabled(false);
		}

	} else if (actionId == 46) {
		__pAttachmentPopup->ShowPopup();
		__pAttachmentPopup->__pPopupHandler = this;
	}
}

void
UiChatForm::DidSelectItemInPopup(int itemIndex, int popupId) {
	this->__pAttachmentPopup->HidePopup();

	if (itemIndex == 0) {
		OpenGallery();
	}
}

/********************** SEND MESSAGE ********************/

void
UiChatForm::SendMessage() {

	String *messageText = null;
	if (__pPosterPanel->GetEditArea()->GetText().GetLength() != 0) {
		messageText = new String(__pPosterPanel->GetEditArea()->GetText());

		AppLog("len: %d content: %S", __pPosterPanel->GetEditArea()->GetText().GetLength(), __pPosterPanel->GetEditArea()->GetText().GetPointer());
	}

	MMessage *pMessage = new MMessage();

	pMessage->SetUid(__userId);

	if (messageText) {
		pMessage->SetText(messageText);
	}

	pMessage->SetOut(1);
	pMessage->SetReadState(0);
	pMessage->SetDelivered(0);
	unsigned long int timestamp = time(NULL);

	pMessage->SetDate(timestamp);
	PostMan::getInstance().SendMessageFromUserWithListener(pMessage, __userId, this);
}

/********************** SCROLL ***********************/

void
UiChatForm::OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type) {
	if (type == SCROLL_END_EVENT_END_TOP) {
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
	if (this->__pListView && this->GetMessages() && this->GetMessages()->GetCount() > 0) {
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


//RefreshableListView

void
UiChatForm::RequestUpdateForIndex(int index, int elementId) {
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
UiChatForm::RequestImageUpdateForIndex(int index, int section, int elementId) {

}

/****************** ATTACHMENT ***********************/

void
UiChatForm::RequestAttachmentDelete(MAttachment *attachment, int index) {
	PostMan::getInstance().RemoveAttachmentAtIndexWithUid(index, __userId);
	this->__pPosterPanel->RemoveAttachment(attachment, index);

	if(PostMan::getInstance().GetAttachmentsForUid(__userId)->GetCount() == 0) {
		this->ResetAttachmentsContainer();
	}
}

void
UiChatForm::ResetAttachmentsContainer() {
	int height = this->__pPosterPanel->GetBounds().height;
	int editAreaHeight = height - itemContentSize;

	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();

	AppLog("editAreaHeight %d", editAreaHeight);

	__pPosterPanel->SetRectangle(FloatRectangle(0, clientRect.height - editAreaHeight,  clientRect.width, editAreaHeight));
	__pListView->SetBounds(Rectangle(0, 100, clientRect.width, clientRect.height - editAreaHeight - 100));
}

void
UiChatForm::RestoreAttachmentContainer() {
	LinkedList *pAttachments = PostMan::getInstance().GetAttachmentsForUid(__userId);

	if (pAttachments->GetCount() > 0) {
		this->__pPosterPanel->AddAttachments(pAttachments);
		this->EnlargeMessengerPanel();
	}
}

void
UiChatForm::EnlargeMessengerPanel() {
	int height = this->__pPosterPanel->GetBounds().height;

	int editAreaHeight = height + itemContentSize;

	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();

	__pPosterPanel->SetRectangle(FloatRectangle(0, clientRect.height - editAreaHeight,  clientRect.width, editAreaHeight));
	__pListView->SetBounds(Rectangle(0, 100, clientRect.width, clientRect.height - editAreaHeight - 100));
}

void
UiChatForm::AddAttachmentToContainer(MAttachment *attachment) {
	this->__pPosterPanel->AddAttachment(attachment);

	if(PostMan::getInstance().GetAttachmentsForUid(__userId)->GetCount() == 1) {
		this->EnlargeMessengerPanel();
	}
}

void
UiChatForm::AddAttachmentsToContainer(LinkedList *pAttachments) {

	bool task = false;
	if (this->__pPosterPanel->__pItems->GetCount() == 0) {
		task = true;
	}

	this->__pPosterPanel->AddAttachments(pAttachments);

	if (task) {
		this->EnlargeMessengerPanel();
	}
}

void
UiChatForm::OpenGallery() {
String mime = L"*/*";
   HashMap extraData;
   extraData.Construct();
   String selectKey = L"http://tizen.org/appcontrol/data/selection_mode";
   String selectVal = L"multiple";
   extraData.Add(&selectKey, &selectVal);

   AppControl* pAc = AppManager::FindAppControlN(L"tizen.gallery",
												 L"http://tizen.org/appcontrol/operation/pick");
   if (pAc)
   {
	  pAc->Start(null, &mime, &extraData, this);
	  delete pAc;
   }
}

void
UiChatForm::OnAppControlCompleteResponseReceived(const Tizen::App::AppId& appId,
                                                 const Tizen::Base::String& operationId,
                                                 const Tizen::App::AppCtrlResult appControlResult,
                                                 const Tizen::Base::Collection::IMap* pExtraData)
{
   if (appId.Equals(String(L"tizen.gallery")) &&
       operationId.Equals(String(L"http://tizen.org/appcontrol/operation/pick")))
   {
      if (appControlResult  == APP_CTRL_RESULT_SUCCEEDED)
      {
         AppLog("Media list retrieving succeeded.");
         // Use the selected media paths
         if (pExtraData)
         {
            IList* pValueList = const_cast< IList* >(dynamic_cast< const IList* >
                                                     (pExtraData->GetValue(String(L"http://tizen.org/appcontrol/data/selected"))));
            if (pValueList)
            {

            	LinkedList *pAttachments = new LinkedList();

               for (int i = 0; i < pValueList->GetCount(); i++)
               {

            	  LinkedList *attachments = PostMan::getInstance().GetAttachmentsForUid(__userId);

            	  if (attachments->GetCount() >= 10) {
            		  continue;
            	  }

                  String* pValue = dynamic_cast< String* >(pValueList->GetAt(i));

                  MAttachment *attachment = new MAttachment();
                  attachment->__pType = new String(PHOTO);

                  pAttachments->Add(attachment);

                  int index = 0;

                  if (attachments && attachments->GetCount() > 0) {
                	  MAttachment *attach = static_cast<MAttachment *>(attachments->GetAt(attachments->GetCount()-1));
                	  index = attach->__tempId + 1;
                  }

                  attachment->__tempId = index;
                  attachment->__pFilePath = pValue;

                  PostMan::getInstance().AddAttachmentWithUid(attachment, this->__userId);

               }

               this->AddAttachmentsToContainer(pAttachments);

            }
         }
      }
      else if (appControlResult  == APP_CTRL_RESULT_FAILED)
      {
         AppLog("Media list retrieving failed.");
      }
      else if (appControlResult  == APP_CTRL_RESULT_CANCELED)
      {
         AppLog("Media list retrieving was canceled.");
      }
      else if (appControlResult == APP_CTRL_RESULT_TERMINATED)
      {
         AppLog("Media list retrieving was terminated.");
      }
      else if (appControlResult == APP_CTRL_RESULT_ABORTED)
      {
         AppLog("Media list retrieving was is aborted.");
      }

   }
}


void
UiChatForm::OnSuccessN(MAttachment *attachment, int uid) {
	if (this->__userId == uid) {

		int index = -1;
		PostMan::getInstance().GetAttachmentsForUid(__userId)->IndexOf(*attachment, index);

		if (index != -1) {
			this->__pPosterPanel->SetAttachmentReady(true, index, attachment);
		}
	}
}

void
UiChatForm::OnErrorN(Error *error, MAttachment *attachment, int uid) {
	if (this->__userId == uid) {

	}
}

void
UiChatForm::OnProgressChanged(MAttachment *attachment, int progress, int uid) {
	if (this->__userId == uid) {

		int index = -1;
		PostMan::getInstance().GetAttachmentsForUid(__userId)->IndexOf(*attachment, index);

		if (index != -1) {
			this->__pPosterPanel->UpdateAttachmentProgress(progress, index, attachment);
		}
	}
}

void
UiChatForm::OnTimerExpired (Timer &timer) {
	this->__isUserPrinting = false;

	delete this->__pPrintingTimer;
	this->__pPrintingTimer = null;
}

void
UiChatForm::NotifyUserTyping() {
	if (__userId > isChatValue) {
		return;
	}

	this->__isUserPrinting = true;
	if (this->__pPrintingTimer) {
		this->__pPrintingTimer->Cancel();
		delete this->__pPrintingTimer;
		this->__pPrintingTimer = null;
	}

	HashMap *params = new HashMap();

	params->Construct();
	params->Add(new String(L"user_id"), new String(AuthManager::getInstance().UserId()->GetPointer()));
	params->Add(new String(L"type"), new String(L"typing"));
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pNotifyUserPrintingOperation) {
		__pNotifyUserPrintingOperation = new RestRequestOperation(NOTIFY_USER_PRINTING, new String(L"messages.setActivity"), params);
		__pNotifyUserPrintingOperation->AddEventListener(this);
		__pNotifyUserPrintingOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__pNotifyUserPrintingOperation);
	}

	this->__pPrintingTimer = new Timer();
	this->__pPrintingTimer->Construct(*this);
	this->__pPrintingTimer->Start(5000);
}

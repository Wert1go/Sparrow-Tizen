/*
 * UiChatForm.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "UiChatForm.h"

#include "SceneRegister.h"
#include "MMessageDescriptor.h"
#include "RestClient.h"
#include "AuthManager.h"
#include "RMessagesResponse.h"
#include "MMessage.h"
#include "MMessageDao.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;

UiChatForm::UiChatForm() {
	result r = E_SUCCESS;

	r = Form::Construct(FORM_STYLE_NORMAL);

	SetFormBackEventListener(this);
	__pMessagesRequestOperation = null;
	__pMessages = null;
	__pListView = null;
}

UiChatForm::~UiChatForm() {
	__pMessagesRequestOperation = null;
}


result
UiChatForm::OnInitializing(void)
{

	AppLog("OnInitializing");
	this->SetName(L"UiChatForm");
	result r = E_SUCCESS;
	Rectangle clientRect;
	clientRect = this->GetClientAreaBounds();

	__pEditArea = new ExpandableEditArea();

	__pEditArea->Construct(
		Rectangle(0, clientRect.height - 110, clientRect.width, 110),
		EXPANDABLE_EDIT_AREA_STYLE_NORMAL,
		EXPANDABLE_EDIT_AREA_TITLE_STYLE_NONE,
		3);

	//__pEditArea->Construct(Rectangle(0, clientRect.height - 110, clientRect.width, 110), INPUT_STYLE_OVERLAY, 5000);

	__pEditArea->AddKeypadEventListener(*this);
	__pEditArea->AddTextEventListener(*this);
	__pEditArea->AddExpandableEditAreaEventListener(*this);

	AddControl(__pEditArea);

	__pListView = new ListView();
	__pListView->Construct(Rectangle(0, 0, clientRect.width, clientRect.height - 110), true, false);
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);

	Color *separatorColor = new (std::nothrow) Color(28, 28, 28, 255);
	__pListView->SetItemDividerColor(*separatorColor);
	delete separatorColor;

	AddControl(__pListView);

	__pItemContext = new ListContextItem();
	__pItemContext->Construct();
	__pItemContext->AddElement(ID_CONTEXT_ITEM_1, L"Test1");
	__pItemContext->AddElement(ID_CONTEXT_ITEM_2, L"Test2");

//	this->SetDialogsList(MDialogDao::getInstance().GetDialogsWithOffsetN(0));
//	SendRequest();

	return r;
}

result
UiChatForm::OnTerminating() {
	result r = E_SUCCESS;
	return r;
}

void
UiChatForm::SetMessages(LinkedList *messages) {
	__pMessages = messages;
}

LinkedList *
UiChatForm::GetMessages() {
	return __pMessages;
}

void
UiChatForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId) {

}

void
UiChatForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}


void
UiChatForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs->GetCount() > 0) {
		Integer *param = static_cast< Integer* > (pArgs->GetAt(0));
		RequestMessagesForUser(param->ToInt());
	}

}

void
UiChatForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
UiChatForm::RequestMessagesForUser(int userId) {
	if (!__pMessagesRequestOperation) {

	}
	HashMap *params = new HashMap();
	String uidString;
	uidString.Format(25, L"%d", userId);

	params->Construct();
	params->Add(new String(L"user_id"), new String(uidString));
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	AppLog("2");
	if (!__pMessagesRequestOperation) {
		__pMessagesRequestOperation = new RestRequestOperation(GET_MESSAGES_HISTORY, new String(L"messages.getHistory"), params);
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
	AppLog("CreateItem %d", index);
	CustomItem *pItem = new CustomItem();
    ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;

    int height = 136;

    pItem->Construct(Dimension(itemWidth, height), style);
    pItem->SetContextItem(__pItemContext);
    MMessage *message = static_cast<MMessage *>(this->GetMessages()->GetAt(index));
    pItem->AddElement(FloatRectangle(0, 0, (float)itemWidth, height), 24, message->GetText()->GetPointer(), false);

//    pItem->SetDimension(new Dimension(itemWidth, height));
//    pItem->SetIndex(index);
//    pItem->AddRefreshListener(this);

//    MDialog *dialog = static_cast<MDialog *>(this->GetDialogsList()->GetAt(index));
//    pItem->SetDialog(dialog);

//    pItem->Init();

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


/***********************************************/

void
UiChatForm::OnSuccessN(RestResponse *result) {
	if(__pMessagesRequestOperation) {
		__pMessagesRequestOperation->AddEventListener(null);
		__pMessagesRequestOperation = null;
	}

	if (result) {
		AppLog("!!!__pMessages1");
	}

	RMessagesResponse *response = static_cast<RMessagesResponse *>(result);
	AppLog("__pMessages:: %d", response->GetMessages()->GetCount());
	this->SetMessages(response->GetMessages());

	MMessage *message = static_cast<MMessage *>(response->GetMessages()->GetAt(0));

	this->SetMessages(MMessageDao::getInstance().GetMessagesForUser(message->GetUid()));

	this->SendUserEvent(333333, 0);
	Tizen::App::App::GetInstance()->SendUserEvent(33333, 0);
	AppLog("__pMessages1");
}

void
UiChatForm::OnErrorN(Error *error) {

}

void
UiChatForm::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	AppLog("__pMessages2");
	if (requestId == 333333 && __pListView) {
		AppLog("__pMessages3");
		this->__pListView->UpdateList();
		this->__pListView->ScrollToItem(this->GetMessages()->GetCount() - 1);
	}
}


void
UiChatForm::OnTextValueChanged(const Tizen::Ui::Control& source) {

}

void
UiChatForm::OnTextValueChangeCanceled(const Tizen::Ui::Control& source) {

}

void
UiChatForm::OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount) {
	FloatRectangle editBounds = source.GetBoundsF();
	this->__pListView->SetBounds(FloatRectangle(editBounds.x, 0, editBounds.width, this->__pListView->GetBounds().height - 30));
	source.SetBounds(FloatRectangle(editBounds.x, this->__pListView->GetBounds().height, editBounds.width, editBounds.height));
	this->Invalidate(true);
}

void
UiChatForm::OnExpandableEditAreaLineRemoved(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount) {
	FloatRectangle editBounds = source.GetBoundsF();
	this->__pListView->SetBounds(FloatRectangle(editBounds.x, 0, editBounds.width, this->__pListView->GetBounds().height + 30));
		source.SetBounds(FloatRectangle(editBounds.x, this->__pListView->GetBounds().height, editBounds.width, editBounds.height));
		this->Invalidate(true);
}

void
UiChatForm::OnKeypadActionPerformed(Control& source, KeypadAction keypadAction)
{
}

void
UiChatForm::OnKeypadClosed(Control& source)
{

	Rectangle clientRect;
	clientRect = this->GetClientAreaBounds();

	this->__pEditArea->SetBounds(Rectangle(0, clientRect.height - 110, clientRect.width, 110));
	this->__pListView->SetBounds(Rectangle(0, 0, clientRect.width, clientRect.height - 110));
	this->Invalidate(true);
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


	this->__pListView->SetBounds(FloatRectangle(editBounds.x, 0, editBounds.width, 720));
	source.SetBounds(FloatRectangle(editBounds.x, 720, editBounds.width, editBounds.height));

	this->Invalidate(true);

	AppLog("%f, %f, %f, %f", editBounds.x, editBounds.y, editBounds.width, editBounds.height);
	AppLog("%f, %f, %f, %f", prevBounds.x, prevBounds.y, prevBounds.width, prevBounds.height);

}

void
UiChatForm::OnKeypadBoundsChanged(Control& source)
{
}

/*
 * UiChatForm.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef UICHATFORM_H_
#define UICHATFORM_H_

#include <FUiCtrlForm.h>
#include <FUi.h>
#include <FApp.h>

#include "RestResponse.h"
#include "Error.h"

#include "IRestRequestListener.h"
#include "RestRequestOperation.h"
#include "UiMessengerPanel.h"
#include "IMessageDeliveryListener.h"
#include "IRefreshableListView.h"

#include "IAttachmentListener.h"
#include "IRequestAttachmentDelete.h"
#include "IPopupHandler.h"

using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;

class MMessage;
class UiChapPanel;
class MDialog;
class MAttachment;
class UiAttachmentListPopup;

class UiChatForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public IRestRequestListener
 , public Tizen::Ui::Controls::IListViewItemEventListener
 , public Tizen::Ui::Controls::IListViewItemProvider
 , public Tizen::Ui::ITextEventListener
 , public Tizen::Ui::Controls::IExpandableEditAreaEventListener
 , public Tizen::Ui::IKeypadEventListener
 , public Tizen::Ui::IActionEventListener
 , public IScrollEventListener
 , public IMessageDeliveryListener
 , public IRefreshableListView
 , public IAttachmentListener
 , public IAppControlResponseListener
 , public IRequestAttachmentDelete
 , public IPopupHandler

{
public:
	UiChatForm();
	virtual ~UiChatForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//listview
    // IListViewItemEventListener
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView &listView, int index, Tizen::Ui::Controls::SweepDirection direction);

	// IListViewItemProvider
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual int GetItemCount(void);

	//scroll

	virtual void OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type);

	// ITextEventListener
	virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source);

	virtual void OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount);
	virtual void OnExpandableEditAreaLineRemoved(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount);

	//keypad

	virtual void OnKeypadActionPerformed(Tizen::Ui::Control& source, Tizen::Ui::KeypadAction keypadAction);
	virtual void OnKeypadClosed(Tizen::Ui::Control& source);
	virtual void OnKeypadOpened(Tizen::Ui::Control& source);
	virtual void OnKeypadWillOpen(Tizen::Ui::Control& source);
	virtual void OnKeypadBoundsChanged(Tizen::Ui::Control& source);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	//Rest
	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	virtual void OnMessageDelivered(int userId, MMessage *message);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	//
	bool IsAlreadyAdded(MMessage *message) ;
	void SendMessage();

	//

	void RequestMessagesForUser(int userId);



	//RefreshableListView
	virtual void RequestUpdateForIndex(int index, int elementId);
	virtual void RequestImageUpdateForIndex(int index, int section, int elementId);

	//Attachment
	virtual void OnSuccessN(MAttachment *attachment, int uid);
	virtual void OnErrorN(Error *error, MAttachment *attachment, int uid);
	virtual void OnProgressChanged(MAttachment *attachment, int progress, int uid);

	virtual void RequestAttachmentDelete(MAttachment *attachment, int index);

	void AddAttachmentToContainer(MAttachment *attachment);
	void AddAttachmentsToContainer(LinkedList *pAttachments);
	void ResetAttachmentsContainer();
	void RestoreAttachmentContainer();
	void EnlargeMessengerPanel();
	//

	//Popup
	virtual void DidSelectItemInPopup(int itemIndex, int popupId);

	void SetMessages(LinkedList *messages);
	LinkedList * GetMessages();

	void ScrollToFirstMessage();
	void ScrollToLastMessage();

	void UpdateCurrentUserOnlineWithValue(int value);
	void SetReadStateWithMessageId(int msgId);
	void MarkUnread();

	void OpenGallery();
	virtual void OnAppControlCompleteResponseReceived(const AppId& appId, const Tizen::Base::String& operationId, AppCtrlResult appControlResult, const Tizen::Base::Collection::IMap* pExtraData);

private:
	static const int ID_CONTEXT_ITEM_1 = 103;
	static const int ID_CONTEXT_ITEM_2 = 104;

	RestRequestOperation *__pMessagesRequestOperation;
	RestRequestOperation *__pMarkAsReadRequestOperation;

	LinkedList *__pMessages;
	ListView *__pListView;
	ListContextItem *__pItemContext;
	Tizen::Ui::Controls::ExpandableEditArea* __pEditArea;

	UiChapPanel *__pChatPanel;
	UiMessengerPanel *__pPosterPanel;
	UiAttachmentListPopup *__pAttachmentPopup;

	void RequestMoreMessagesFromMid(int mid);

	int __userId;
	int __chatId;

	int __lastMessageId;
	MDialog *__pDialog;

	bool __isActive;

};

#endif /* UICHATFORM_H_ */

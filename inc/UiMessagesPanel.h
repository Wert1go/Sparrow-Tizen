/*
 * UiMessagesPanel.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef UIMESSAGESPANEL_H_
#define UIMESSAGESPANEL_H_

#include <FUi.h>
#include "IRefreshableListView.h"
#include "IRestRequestListener.h"
#include "RestRequestOperation.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

class UiMessagesPanel
 : public Panel
 , public Tizen::Ui::Controls::IListViewItemEventListener
 , public Tizen::Ui::Controls::IListViewItemProvider
 , public IRefreshableListView
 , public IRestRequestListener
 , public Tizen::Ui::Controls::ISearchBarEventListener
 , public Tizen::Ui::ITextEventListener
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::IActionEventListener
 , public IScrollEventListener
 , public ITimerEventListener
{
public:
	UiMessagesPanel();
	virtual ~UiMessagesPanel();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	    // IListViewItemEventListener
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView &listView, int index, Tizen::Ui::Controls::SweepDirection direction);

	    // IListViewItemProvider
    virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual int GetItemCount(void);

	virtual void RequestUpdateForIndex(int index, int elementId);
	virtual void RequestImageUpdateForIndex(int index, int section, int elementId);
	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

    // ISearchBarEventListener
    virtual void OnSearchBarModeChanged(Tizen::Ui::Controls::SearchBar& source, Tizen::Ui::Controls::SearchBarMode mode){};
    virtual void OnSearchBarContentAreaResized(Tizen::Ui::Controls::SearchBar& source, Tizen::Graphics::Dimension& size) {};
    virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
    virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source){};

    virtual void OnScrollEndReached(Tizen::Ui::Control& source, Tizen::Ui::Controls::ScrollEndEvent type);

    void SetSearchMode(bool state);
    bool SearchModeIsActive();
    void SetSearchModeCode(int code);

    virtual void OnTimerExpired (Timer &timer);

    void ShowNewMessageButton();
    void HideNewMessageButton();

private:
    bool __isSearchMode;
    int __searchModeCode;

private:
    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	void SetDialogsList(LinkedList *list);
	LinkedList * GetDialogsList();

	void SetSearchDialogsList(LinkedList *list);
	LinkedList * GetSearchDialogsList();

	void SetSearchMessagesList(LinkedList *list);
	LinkedList * GetSearchMessagesList();

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);
	void UpdateItemListWithUserId(int userId, int value);
	void SetReadStateWithMessageId(int msgId);
	void UpdateUnreadCount();

	void SearchDialogs(String searchText);
	void SearchMessages(String searchText);

private:
	static const int ID_CONTEXT_ITEM_1 = 103;
	static const int ID_CONTEXT_ITEM_2 = 104;

	Tizen::Ui::Controls::ListView* __pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;
	Button *__pAddButton;

	LinkedList *__pDialogsList;

	LinkedList *__pSearchDialogsList;
	LinkedList *__pSearchMessagesList;

	RestRequestOperation *__pDialogRequestOperation;
	RestRequestOperation *__pSearchDialogRequestOperation;
	RestRequestOperation *__pSearchMessageRequestOperation;

	SearchBar *__pSearchBar;
	void SendRequest(int offset = 0);
	virtual result OnDraw(void);

	Timer *__pListUpdateTimer;
};

#endif /* UIMESSAGESPANEL_H_ */

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

class UiMessagesPanel
 : public Panel
 , public Tizen::Ui::Controls::IListViewItemEventListener
 , public Tizen::Ui::Controls::IListViewItemProvider
 , public IRefreshableListView
 , public IRestRequestListener
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

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

private:
	void SetDialogsList(LinkedList *list);
	LinkedList * GetDialogsList();

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

private:
	static const int ID_CONTEXT_ITEM_1 = 103;
	static const int ID_CONTEXT_ITEM_2 = 104;

	Tizen::Ui::Controls::ListView* __pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;

	LinkedList *__pDialogsList;
	RestRequestOperation *__pDialogRequestOperation;
	void SendRequest();
};

#endif /* UIMESSAGESPANEL_H_ */

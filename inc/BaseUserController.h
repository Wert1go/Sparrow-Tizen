/*
 * BaseUserController.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef BASEUSERCONTROLLER_H_
#define BASEUSERCONTROLLER_H_

#include <FBaseObject.h>
#include <FUi.h>
#include <FBase.h>
#include "IRefreshableListView.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class BaseUserController
 : public Tizen::Base::Object
 , public Tizen::Ui::Controls::IGroupedListViewItemEventListener
 , public Tizen::Ui::Controls::IGroupedListViewItemProvider
 , public IRefreshableListView
 {
public:
	BaseUserController();
	virtual ~BaseUserController();

	void InitData();

   // IGroupedListViewItemEventListener
	virtual void OnGroupedListViewContextItemStateChanged(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListItemStatus state);
	virtual void OnGroupedListViewItemSwept(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Tizen::Ui::Controls::SweepDirection direction);

	// IGroupedListViewItemProvider
	virtual int GetGroupCount(void);
	virtual int GetItemCount(int groupIndex);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int groupIndex, int itemIndex, int itemWidth);
	virtual Tizen::Ui::Controls::GroupItem* CreateGroupItem(int groupIndex, int itemWidth);
	virtual bool DeleteItem(int groupIndex, int itemIndex, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual bool DeleteGroupItem(int groupIndex, Tizen::Ui::Controls::GroupItem* pItem, int itemWidth);

	//List view refresh
	virtual void RequestImageUpdateForIndex(int index, int section, int elementId);
	virtual void RequestUpdateForIndex(int index, int elementId);

	virtual void RequestUiUpdate(int code, IList *pArgs) = 0;

	void SplitUsersToSections();

public:
	Tizen::Ui::Controls::GroupedListView* __pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;

	String *__pScrollIndex;

	LinkedList *__pUsersList;

	LinkedList *__pSectionItemsList;
	LinkedList *__pSectionTitlesList;
};

#endif /* BASEUSERCONTROLLER_H_ */

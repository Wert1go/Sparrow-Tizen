/*
 * UiUsersPanel.h
 *
 *  Created on: Nov 20, 2013
 *      Author: developer
 */

#ifndef UIUSERSPANEL_H_
#define UIUSERSPANEL_H_

#include <FUi.h>
#include "IRefreshableListView.h"
#include "IRestRequestListener.h"
#include "RestRequestOperation.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;

class RestResponse;
class Error;

class UiUsersPanel
 : public Panel
 , public Tizen::Ui::Controls::IGroupedListViewItemEventListener
 , public Tizen::Ui::Controls::IGroupedListViewItemProvider
 , public IRefreshableListView
 , public IRestRequestListener
 , public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	UiUsersPanel();
	virtual ~UiUsersPanel();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

private:
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

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	void RequestUpdateForIndex(int index, int elementId);

private:
	Tizen::Ui::Controls::GroupedListView* __pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;

	LinkedList *__pUsersList;
	RestRequestOperation *__pUserRequestOperation;

	void RequestUsers();

};

#endif /* UIUSERSPANEL_H_ */

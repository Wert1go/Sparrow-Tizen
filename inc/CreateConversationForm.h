/*
 * CreateConversationForm.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef CREATECONVERSATIONFORM_H_
#define CREATECONVERSATIONFORM_H_

#include "BaseUserController.h"
#include "IRestRequestListener.h"

class RestResponse;
class Error;
class RestRequestOperation;
class MDialog;

class CreateConversationForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::IActionEventListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::Controls::IListViewItemEventListener
 , public Tizen::Ui::Controls::IListViewItemProvider
 , public IRefreshableListView
 , public IRestRequestListener

{
public:
	CreateConversationForm();
	virtual ~CreateConversationForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void RequestUiUpdate(int code, IList *pArgs);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList *pArgs);

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

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	void CreateChat();
	virtual result OnDraw(void);
private:
	LinkedList *__pUsersList;
	ListView *__pListView;
	EditArea* __pEditArea;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;

	RestRequestOperation *__pChatCreateOperation;
	MDialog *__pDialog;
};

#endif /* CREATECONVERSATIONFORM_H_ */

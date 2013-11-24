/*
 * SelectChatUsersForm.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef SELECTCHATUSERSFORM_H_
#define SELECTCHATUSERSFORM_H_

#include <FUiCtrlForm.h>
#include "BaseUserController.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;

class SelectChatUsersForm
	: public BaseUserController
	, public Tizen::Ui::Controls::Form
	, public Tizen::Ui::Scenes::ISceneEventListener
	, public IFastScrollListener
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	SelectChatUsersForm();
	virtual ~SelectChatUsersForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void RequestUiUpdate(int code, IList *pArgs);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	ListItemBase* CreateItem(int groupIndex, int itemIndex, int itemWidth);
	void OnGroupedListViewItemStateChanged(GroupedListView &listView, int groupIndex, int itemIndex, int elementId, ListItemStatus state);

	ListAnnexStyle GetCurrentAnnexStyle();


private:
	LinkedList *__pSelectedUsers;
};

#endif /* SELECTCHATUSERSFORM_H_ */

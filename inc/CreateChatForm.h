/*
 * CreateChatForm.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef CREATECHATFORM_H_
#define CREATECHATFORM_H_

#include <FUiCtrlForm.h>
#include "BaseUserController.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

class CreateChatForm
 : public Tizen::Ui::Controls::Form
 , public BaseUserController
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public IFastScrollListener
 , public Tizen::Ui::Controls::ISearchBarEventListener
 , public Tizen::Ui::ITextEventListener
 , public Tizen::Ui::IActionEventListener
 , public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	CreateChatForm();
	virtual ~CreateChatForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

    // ISearchBarEventListener
    virtual void OnSearchBarModeChanged(Tizen::Ui::Controls::SearchBar& source, Tizen::Ui::Controls::SearchBarMode mode){};
    virtual void OnSearchBarContentAreaResized(Tizen::Ui::Controls::SearchBar& source, Tizen::Graphics::Dimension& size) {};
    virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
    virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source){};

    virtual void OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListItemStatus state);
    virtual int GetGroupCount(void);
	virtual int GetItemCount(int groupIndex);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int groupIndex, int itemIndex, int itemWidth);
	virtual Tizen::Ui::Controls::GroupItem* CreateGroupItem(int groupIndex, int itemWidth);

    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual void RequestUiUpdate(int code, IList *pArgs);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

private:
	SearchBar *__pSearchBar;
	ListContextItem *__pItemContextCommon;

};

#endif /* CREATECHATFORM_H_ */

/*
 * SearchUserPanel.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef SEARCHUSERPANEL_H_
#define SEARCHUSERPANEL_H_

#include <FUiCtrlPanel.h>
#include "BaseUserController.h"
#include "IRestRequestListener.h"
#include "RestRequestOperation.h"

using namespace Tizen::Base::Runtime;

class SearchUserPanel
 : public Tizen::Ui::Controls::Panel
 , public BaseUserController
 , public IRestRequestListener
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::Controls::ISearchBarEventListener
 , public Tizen::Ui::ITextEventListener
 , public ITimerEventListener
 {
public:
	SearchUserPanel();
	virtual ~SearchUserPanel();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	// ISearchBarEventListener
	virtual void OnSearchBarModeChanged(Tizen::Ui::Controls::SearchBar& source, Tizen::Ui::Controls::SearchBarMode mode){};
	virtual void OnSearchBarContentAreaResized(Tizen::Ui::Controls::SearchBar& source, Tizen::Graphics::Dimension& size) {};
	virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source){};

	virtual void OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Tizen::Ui::Controls::ListItemStatus state);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void RequestUiUpdate(int code, IList *pArgs);

	virtual void OnTimerExpired (Timer &timer);

private:
	RestRequestOperation *__pUserSearchOperation;
	RestRequestOperation *__pUserSuggestOperation;
	RestRequestOperation *__pUserRequestsOperation;

	SearchBar *__pSearchBar;

	LinkedList *__pUsersSuggest;
	LinkedList *__pUsersRequests;
	LinkedList *__pUsersSearchResults;

	Timer *__pListUpdateTimer;

	void SplitUsersToSections();

	void SearchUserWithString(String string);
	void GetUserRequests();
	void GetSuggests();
};

#endif /* SEARCHUSERPANEL_H_ */

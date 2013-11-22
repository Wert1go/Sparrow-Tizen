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

 , public IRefreshableListView
 , public IRestRequestListener
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public IFastScrollListener
 , public Tizen::Ui::Controls::ISearchBarEventListener
 , public Tizen::Ui::ITextEventListener
 , public Tizen::Ui::IActionEventListener

{
public:
	UiUsersPanel();
	virtual ~UiUsersPanel();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	void SetCurrentDisplayMode(int mode);
private:

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	virtual void RequestImageUpdateForIndex(int index, int section, int elementId);
	virtual void RequestUpdateForIndex(int index, int elementId);

    // ISearchBarEventListener
    virtual void OnSearchBarModeChanged(Tizen::Ui::Controls::SearchBar& source, Tizen::Ui::Controls::SearchBarMode mode){};
    virtual void OnSearchBarContentAreaResized(Tizen::Ui::Controls::SearchBar& source, Tizen::Graphics::Dimension& size) {};
    virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
    virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source){};

    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
	Tizen::Ui::Controls::GroupedListView* __pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;



	RestRequestOperation *__pUserRequestOperation;
	SearchBar *__pSearchBar;

	void RequestUsers();
	void SplitUsersToSections();

};

#endif /* UIUSERSPANEL_H_ */

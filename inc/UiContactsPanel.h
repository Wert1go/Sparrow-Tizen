/*
 * UiContactsPanel.h
 *
 *  Created on: Dec 16, 2013
 *      Author: developer
 */

#ifndef UICONTACTSPANEL_H_
#define UICONTACTSPANEL_H_

#include <FUi.h>
#include "BaseUserController.h"
#include "IRestRequestListener.h"

using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;

class RestResponse;
class Error;
class RestRequestOperation;

class UiContactsPanel
 : public Tizen::Ui::Controls::Panel
 , public Tizen::Ui::IActionEventListener
 , public IRestRequestListener
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public BaseUserController
 , public IFastScrollListener
 , public Tizen::Ui::Controls::ISearchBarEventListener
 , public Tizen::Ui::ITextEventListener
   {
public:
	UiContactsPanel();
	virtual ~UiContactsPanel();

	virtual result OnDraw(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

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

	virtual void RequestUiUpdate(int code, IList *pArgs);

	virtual void OnFastScrollIndexSelected(Tizen::Ui::Control& source, Tizen::Base::String& index);

	void SplitUsersToSections();
	void SplitUsersToSectionsByAlphabet();

	void ImportContacts();
	void SyncContacts();

	void UpdateScreen();

private:
	Tizen::Ui::Controls::Button* pImportButton;
	Tizen::Ui::Controls::TextBox* pInfoLabel;

	SearchBar *__pSearchBar;

	RestRequestOperation *__pUserImportOperation;

	LinkedList *__pContactsList;
};

#endif /* UICONTACTSPANEL_H_ */

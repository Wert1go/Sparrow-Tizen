/*
 * SettingsForm.h
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#ifndef SETTINGSFORM_H_
#define SETTINGSFORM_H_

#include <FUi.h>
#include <FNet.h>
#include <FMedia.h>
#include "IRestRequestListener.h"

#include "RestResponse.h"
#include "Error.h"
#include "MUser.h"
#include "RestRequestOperation.h"
#include "IImageLoadingListener.h"

using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

class SettingsForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public IRestRequestListener
 , public IImageLoadingListener
 , public Tizen::Ui::Controls::ISectionTableViewItemEventListener
 , public Tizen::Ui::Controls::ISectionTableViewItemProviderF
 , public Tizen::Ui::IActionEventListener
 , public Tizen::Ui::Scenes::ISceneEventListener
   {
public:
	SettingsForm();
	virtual ~SettingsForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void SendRequest(void);
	virtual void OnSuccessN(RestResponse *user);
	virtual void OnErrorN(Error *error);
	virtual void OnImageLoadedN(Bitmap *result, Integer *code);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
										   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
		virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
											const Tizen::Ui::Scenes::SceneId& nextSceneId);
	MUser *__user;
	Tizen::Graphics::Bitmap *__bitmap;

	virtual result OnDraw(void);


	//ISectionTableViewItemProviderF
	virtual int GetSectionCount(void);
	virtual int GetItemCount(int sectionIndex);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItem(int sectionIndex, int itemIndex, float itemWidth);
	virtual bool DeleteItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual void UpdateItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual Tizen::Base::String GetSectionHeader(int sectionIndex);
	virtual bool HasSectionHeader(int sectionIndex);
	virtual Tizen::Base::String GetSectionFooter(int sectionIndex);
	virtual bool HasSectionFooter(int sectionIndex);
	virtual float GetDefaultItemHeight(void);

	Tizen::Ui::Controls::TextBox* pInfoLabel;

	//ISectionTableViewItemEventListener

	virtual void OnSectionTableViewItemStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem, Tizen::Ui::Controls::TableViewItemStatus status);
	virtual void OnSectionTableViewContextItemActivationStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewContextItem* pContextItem, bool activated);


private:
	RestRequestOperation *__userRequestOperation;

	void UpdateInterfaceForCurrentUser();

	Tizen::Ui::Controls::SectionTableView* __pSectionTableView;
	Button *__pExitButton;

};

#endif /* SETTINGSFORM_H_ */

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

class SettingsForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public IRestRequestListener
 , public IImageLoadingListener
 , public Tizen::Ui::Controls::ISectionTableViewItemEventListener
 , public Tizen::Ui::Controls::ISectionTableViewItemProviderF
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

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	MUser *__user;
	Tizen::Graphics::Bitmap *__bitmap;

	virtual result OnDraw(void);
	virtual void OnImageLoadedN(Bitmap *result);

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

	//ISectionTableViewItemEventListener

	virtual void OnSectionTableViewItemStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem, Tizen::Ui::Controls::TableViewItemStatus status);
	virtual void OnSectionTableViewContextItemActivationStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewContextItem* pContextItem, bool activated);


private:
	RestRequestOperation *__userRequestOperation;

	void UpdateInterfaceForCurrentUser();

	Tizen::Ui::Controls::SectionTableView* __pSectionTableView;

};

#endif /* SETTINGSFORM_H_ */

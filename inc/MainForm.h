/*
 * MainForm.h
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#ifndef MAINFORM_H_
#define MAINFORM_H_

#include <FUi.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Base::Collection;

extern const int SEARCH_MESSAGE_MODE;
extern const int SEARCH_DIALOG_MODE;

#include "IRestRequestListener.h"

class RestRequestOperation;
class RestResponse;
class Error;

class MainForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::IActionEventListener
 , public IRestRequestListener

 {
public:
	MainForm();
	virtual ~MainForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	void UpdateUnreadCount(int count = -1);

	void RequestUnreadCount();
	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	void RecreateItems();
	virtual result OnDraw(void);

	HeaderItem *messageItem;
	HeaderItem *contactsItem;
	HeaderItem *searchItem;
	HeaderItem *settingsItem;

	FooterItem *friendsItem;
	FooterItem *friendsOnlineItem;
	FooterItem *footerContactsItem;


	void SetSearchMode(bool state);
	bool SearchModeIsActive();
	void SetSearchModeCode(int code);

private:
	Header* __pHeader;

	virtual void OnSuccessN(RestResponse *user);
	virtual void OnErrorN(Error *error);

	RestRequestOperation *__pGetUnreadCountOperation;
	bool __isSearchMode;
	int __searchModeCode;

	int __normalIndex;
	int __searchIndex;

	int __footerIndex;
	int __lastSelectedIndex;

	int __unreadCount;

};

#endif /* MAINFORM_H_ */

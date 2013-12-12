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

class MainForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::IActionEventListener

 {
public:
	MainForm();
	virtual ~MainForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	void UpdateUnreadCount();
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

private:
	Header* __pHeader;
};

#endif /* MAINFORM_H_ */

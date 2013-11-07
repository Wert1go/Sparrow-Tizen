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
#include "IRestRequestListener.h"

#include "RestResponse.h"
#include "Error.h"
#include "User.h"
#include "RestRequestOperation.h"

class SettingsForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public IRestRequestListener
   {
public:
	SettingsForm();
	virtual ~SettingsForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void SendRequest(void);
	virtual void OnSuccessN(RestResponse *user);
	virtual void OnErrorN(Error *error);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	User *__user;

private:
	RestRequestOperation *__userRequestOperation;

};

#endif /* SETTINGSFORM_H_ */

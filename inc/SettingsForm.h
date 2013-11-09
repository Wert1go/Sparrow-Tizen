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
   {
public:
	SettingsForm();
	virtual ~SettingsForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void SendRequest(void);
	virtual void OnSuccessN(RestResponse *user);
	virtual void OnErrorN(Error *error);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	MUser *__user;
	Tizen::Graphics::Bitmap *__bitmap;

	virtual result OnDraw(void);
	virtual void OnImageLoadedN(Bitmap *result);

private:
	RestRequestOperation *__userRequestOperation;

	void UpdateInterfaceForCurrentUser();

};

#endif /* SETTINGSFORM_H_ */

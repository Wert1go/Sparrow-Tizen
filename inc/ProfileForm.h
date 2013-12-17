/*
 * ProfileForm.h
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#ifndef PROFILEFORM_H_
#define PROFILEFORM_H_

#include <FUi.h>
#include <FBase.h>
#include <FMessaging.h>
#include "IRestRequestListener.h"


#include "IImageLoadingListener.h"

class MUser;
class RestResponse;
class Error;
class RestRequestOperation;

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Messaging;

class ProfileForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public IRestRequestListener
 , public Tizen::Ui::IActionEventListener
 , public IImageLoadingListener
 , public Tizen::Messaging::ISmsListener
   {
public:
	ProfileForm();
	virtual ~ProfileForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	virtual void OnImageLoadedN(Bitmap *result, Integer *code);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
	virtual result OnDraw(void);

	void UpdateScreenForUser();

	void Call();
	void SendInvite();

	virtual void OnSmsMessageSent(result r);

private:
	MUser *__pUser;
	Bitmap *__pUserAvatar;
	Button *__pAddButton;

	Button *__pCallButton;

	Button *__pRemoveButton;
	Tizen::Ui::Controls::TextBox *__pAddLabel;

	RestRequestOperation *__pAddToFriendsOperation;
	RestRequestOperation *__pDeclineFriendOperation;
	RestRequestOperation *__pDeleteFriendOperation;
};

#endif /* PROFILEFORM_H_ */

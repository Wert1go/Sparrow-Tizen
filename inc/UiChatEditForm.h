/*
 * UiChatEditForm.h
 *
 *  Created on: Dec 18, 2013
 *      Author: developer
 */

#ifndef UICHATEDITFORM_H_
#define UICHATEDITFORM_H_

#include <FUi.h>

#include "IRestRequestListener.h"

class MDialog;
class RestRequestOperation;
class RestResponse;

class UiChatEditForm
 : public Tizen::Ui::Controls::Form
, public Tizen::Ui::Scenes::ISceneEventListener
, public Tizen::Ui::IActionEventListener
, public Tizen::Ui::Controls::IFormBackEventListener
, IRestRequestListener
   {
public:
	UiChatEditForm();
	virtual ~UiChatEditForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void OnSuccessN(RestResponse *result);

	void QuitConversation();
	virtual result OnDraw(void);

private:
	RestRequestOperation *__pQuitConversationOperation;
	Tizen::Ui::Controls::Button* __pQuitButton;
	MDialog *__pDialog;

};

#endif /* UICHATEDITFORM_H_ */

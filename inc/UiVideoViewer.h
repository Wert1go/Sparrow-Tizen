/*
 * UiVideoViewer.h
 *
 *  Created on: Dec 14, 2013
 *      Author: developer
 */

#ifndef UIVIDEOVIEWER_H_
#define UIVIDEOVIEWER_H_

#include <FUi.h>
#include "IRestRequestListener.h"

class RestRequestOperation;
class RestResponse;
class Error;

class UiVideoViewer
 : public Tizen::Ui::Controls::Form
 , public IRestRequestListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	UiVideoViewer();
	virtual ~UiVideoViewer();

	virtual bool Initialize(void);

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

private:
	//Rest
	virtual void OnSuccessN(RestResponse *result);
	virtual void OnErrorN(Error *error);

	RestRequestOperation *__pLoadingVideoInfoOperation;
};

#endif /* UIVIDEOVIEWER_H_ */

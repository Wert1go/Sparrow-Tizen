/*
 * UiImageViewer.h
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#ifndef UIIMAGEVIEWER_H_
#define UIIMAGEVIEWER_H_

#include <FUi.h>
#include "IImageLoadingListener.h"

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class Error;

class UiImageViewer
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IGalleryItemProvider
 , public Tizen::Ui::Controls::IGalleryEventListener
 , public IImageLoadingListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::Scenes::ISceneEventListener


{
public:
	UiImageViewer();
	virtual ~UiImageViewer();

	virtual bool Initialize(void);

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

//		virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	// IGalleryItemEventListener
	virtual void OnGalleryCurrentItemChanged (Tizen::Ui::Controls::Gallery &view, int index);
	virtual void OnGalleryItemClicked(Tizen::Ui::Controls::Gallery& listView, int index);
	virtual void OnGallerySlideShowStarted(Tizen::Ui::Controls::Gallery& gallery){};
	virtual void OnGallerySlideShowStopped(Tizen::Ui::Controls::Gallery& gallery){};

	//IGalleryItemProvider
	virtual Tizen::Ui::Controls::GalleryItem* CreateItem(int index);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::GalleryItem* pItem);
	virtual int GetItemCount(void);

	virtual void OnImageLoadedN(Bitmap *result, Integer *code);
	virtual void OnErrorN(Error *error);

	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	//Scenes
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);

private:
	Bitmap *__pPresentingImage;
	String *__pImageUrl;
	Tizen::Ui::Controls::Gallery* __pGallery;
};

#endif /* UIIMAGEVIEWER_H_ */

/*
 * UiImageViewer.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#include "UiImageViewer.h"
#include <FApp.h>
#include "Error.h"
#include "ImageCache.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;

UiImageViewer::UiImageViewer() {


	__pImageUrl = null;
	__pPresentingImage = null;
}

UiImageViewer::~UiImageViewer() {
	if (this->__pPresentingImage) {
		delete this->__pPresentingImage;
		this->__pPresentingImage = null;
	}
}

bool
UiImageViewer::Initialize(void) {
	result r = E_SUCCESS;
	r = Form::Construct(FORM_STYLE_NORMAL);
	this->SetName(L"UiImageViewer");
	SetFormBackEventListener(this);

	return true;
}

result
UiImageViewer::OnInitializing(void) {
	result r = E_SUCCESS;

	FloatRectangle clientRect;
	clientRect = this->GetClientAreaBoundsF();

	__pGallery = new Gallery();
	__pGallery->Construct(clientRect);
	__pGallery->SetBackgroundColor(Color(8, 8, 8, 255));
	__pGallery->SetItemProvider(*this);
	__pGallery->AddGalleryEventListener(*this);

	AddControl(__pGallery);

	__pGallery->SetTextOfEmptyGallery(L"Loading...");

	return r;
}

result
UiImageViewer::OnTerminating(void) {
	result r = E_SUCCESS;
	ImageCache::getInstance().CancelLoadingForTarget(this);

	return r;
}

void
UiImageViewer::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs && pArgs->GetCount() > 0) {
		__pImageUrl = static_cast<String *>(pArgs->GetAt(0));
		ImageCache::getInstance().LoadImageForTarget(__pImageUrl, this);
	}

}

void
UiImageViewer::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
UiImageViewer::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

// ******************** IMAGE LOADING ***********************/

void
UiImageViewer::OnImageLoadedN(Bitmap *result, Integer *code) {
	if (__pPresentingImage) {
		delete __pPresentingImage;
		__pPresentingImage = null;
	}

	__pPresentingImage = result;

	this->SendUserEvent(777, 0);
	Tizen::App::App::GetInstance()->SendUserEvent(777, 0);
}

void
UiImageViewer::OnErrorN(Error *error) {
	//do nothing
}

void
UiImageViewer::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == 777) {
		__pGallery->UpdateGallery();
		Invalidate(true);
	}
}

/* ************************* GALLERY ************************/

int
UiImageViewer::GetItemCount(void)
{
	if(this->__pPresentingImage) {
		return 1;
	} else {
		return 0;
	}
}

GalleryItem*
UiImageViewer::CreateItem(int index)
{
	GalleryItem* pGallery = new (std::nothrow) GalleryItem();
	pGallery->Construct(*this->__pPresentingImage);
	return pGallery;
}

bool
UiImageViewer::DeleteItem(int index, GalleryItem* pItem)
{
	delete pItem;
	return true;
}

void
UiImageViewer::OnGalleryCurrentItemChanged(Gallery& view, int index)
{

}

void
UiImageViewer::OnGalleryItemClicked(Gallery& view, int index)
{

}



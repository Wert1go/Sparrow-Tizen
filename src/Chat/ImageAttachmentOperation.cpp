/*
 * ImageAttachmentOperation.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#include "ImageAttachmentOperation.h"
#include <FBase.h>
#include <FApp.h>

#include "RestClient.h"

#include "RestRequestOperation.h"

#include "RImageUploadServerDescriptor.h"
#include "RImageUploadDataDescriptor.h"
#include "RImageUploadServerResponse.h"

#include "AuthManager.h"
#include "RestResponse.h"
#include "Error.h"

using namespace Tizen::App;

ImageAttachmentOperation::ImageAttachmentOperation() {
	__pGetUploadServerOperation = null;
	__pSaveImageOnServerOperation = null;
	__pUploadImageOperation = null;

	__IsRunning = false;
}

ImageAttachmentOperation::~ImageAttachmentOperation() {
	if (__pGetUploadServerOperation) {
		__pGetUploadServerOperation->AddEventListener(null);
		__pGetUploadServerOperation = null;
	}

	if (__pSaveImageOnServerOperation) {
		__pSaveImageOnServerOperation->AddEventListener(null);
		__pSaveImageOnServerOperation = null;
	}

	if (__pUploadImageOperation) {
		__pUploadImageOperation->AddEventListener(null);
		__pUploadImageOperation = null;
	}

}

void
ImageAttachmentOperation::GetUploadServer() {
	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pGetUploadServerOperation) {
		__pGetUploadServerOperation = new RestRequestOperation(ATTACHMENT_IMAGE_GET_SERVER, new String(L"photos.getMessagesUploadServer"), params);
		__pGetUploadServerOperation->AddEventListener(this);
		__pGetUploadServerOperation->SetResponseDescriptor(new RImageUploadServerDescriptor());
		__pGetUploadServerOperation->__isSync = true;
		RestClient::getInstance().PerformOperation(__pGetUploadServerOperation);
	}
}

void
ImageAttachmentOperation::UploadImageToUrl(String *url) {
	HashMap *params = new HashMap();
	params->Construct();
//	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	HashMap *files = new HashMap();
	files->Construct();

	files->Add(new String(L"photo"), new String(App::GetInstance()->GetAppResourcePath() + L"Main.png"));

	if (!__pUploadImageOperation) {
		__pUploadImageOperation = new RestRequestOperation(url, ATTACHMENT_IMAGE_UPLOAD, null, params, files);
		__pUploadImageOperation->AddEventListener(this);
		__pUploadImageOperation->SetResponseDescriptor(new RImageUploadDataDescriptor());
		__pUploadImageOperation->__isSync = true;
		RestClient::getInstance().PerformOperation(__pUploadImageOperation);
	}
}

void
ImageAttachmentOperation::SavePhotoOnServer(String *photoData, String *server, String *hash) {
	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"photo"), photoData);
	params->Add(new String(L"server"), server);
	params->Add(new String(L"hash"), hash);

	if (!__pSaveImageOnServerOperation) {
		__pSaveImageOnServerOperation = new RestRequestOperation(ATTACHMENT_IMAGE_SAVE, new String(L"photos.saveMessagesPhoto"), params);
		__pSaveImageOnServerOperation->AddEventListener(this);
		__pSaveImageOnServerOperation->SetResponseDescriptor(new RImageUploadDataDescriptor());
		__pSaveImageOnServerOperation->__isSync = true;
		RestClient::getInstance().PerformOperation(__pSaveImageOnServerOperation);
	}
}

void
ImageAttachmentOperation::OnSuccessN(RestResponse *result) {
	if (result->GetOperationCode() == ATTACHMENT_IMAGE_GET_SERVER) {

		RImageUploadServerResponse *response = static_cast<RImageUploadServerResponse *>(result);

		if (response->__pUploadServer) {
			this->UploadImageToUrl(response->__pUploadServer);
		}

	} else if (result->GetOperationCode() == ATTACHMENT_IMAGE_UPLOAD) {
		RImageUploadServerResponse *response = static_cast<RImageUploadServerResponse *>(result);

		if (response->__pPhotoData) {
			this->SavePhotoOnServer(
					response->__pPhotoData,
					response->__pServerData,
					response->__pHashData
					);
		}
	}
}

void
ImageAttachmentOperation::OnErrorN(Error *error) {

}

void
ImageAttachmentOperation::Perform() {
	this->GetUploadServer();
}

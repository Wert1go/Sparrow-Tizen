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
#include "RImageSaveDescriptor.h"

#include "IAttachmentListener.h"
#include "MAttachment.h"

using namespace Tizen::App;

ImageAttachmentOperation::ImageAttachmentOperation() {
	__pGetUploadServerOperation = null;
	__pSaveImageOnServerOperation = null;
	__pUploadImageOperation = null;
	__pAttachmentListener = null;
	__pAttachment = null;
	__IsRunning = false;
	__uid = 0;
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

	if (__pAttachmentListener) {
		__pAttachmentListener = null;
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

	HashMap *files = new HashMap();
	files->Construct();

	if (this->__pAttachment->__pFilePath) {
		files->Add(new String(L"photo"), this->__pAttachment->__pFilePath);
	}

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
		__pSaveImageOnServerOperation->SetResponseDescriptor(new RImageSaveDescriptor());
		__pSaveImageOnServerOperation->__isSync = true;
		RestClient::getInstance().PerformOperation(__pSaveImageOnServerOperation);
	}
}

void
ImageAttachmentOperation::OnSuccessN(RestResponse *result) {

	RImageUploadServerResponse *response = static_cast<RImageUploadServerResponse *>(result);

	if (result->GetOperationCode() == ATTACHMENT_IMAGE_GET_SERVER) {
		if (response->__pUploadServer) {
			this->UploadImageToUrl(response->__pUploadServer);
		}

	} else if (result->GetOperationCode() == ATTACHMENT_IMAGE_UPLOAD) {


		if (response->__pPhotoData) {
			this->SavePhotoOnServer(
					response->__pPhotoData,
					response->__pServerData,
					response->__pHashData
					);
		}
	} else {
		MAttachment *resultAttachment = response->__pAttachment;

		if (resultAttachment) {
			this->__pAttachment->__id = resultAttachment->__id;
			this->__pAttachment->__pType = resultAttachment->__pType;
			this->__pAttachment->__ownerId = resultAttachment->__ownerId;
			this->__pAttachment->__date = resultAttachment->__date;
			this->__pAttachment->__pAccessKey = resultAttachment->__pAccessKey;
			this->__pAttachment->__pPhoto130 = resultAttachment->__pPhoto130;
			this->__pAttachment->__pPhoto604 = resultAttachment->__pPhoto604;
			this->__pAttachment->__width = resultAttachment->__width;
			this->__pAttachment->__height = resultAttachment->__height;
			this->__pAttachment->__mid = resultAttachment->__mid;

			AppLog("%d :: %d", resultAttachment->__ownerId, resultAttachment->__id);
		}

		__IsRunning = false;
		if (__pAttachmentListener) {
			__pAttachmentListener->OnSuccessN(this->__pAttachment, this->__uid);
		}
	}
}

void
ImageAttachmentOperation::OnErrorN(Error *error) {
	__IsRunning = false;
	if (__pAttachmentListener) {
		__pAttachmentListener->OnErrorN(error, this->__pAttachment, this->__uid);
	}
}

void
ImageAttachmentOperation::OnProgressChanged(int progress) {
	if (__pAttachmentListener) {
		__pAttachmentListener->OnProgressChanged(this->__pAttachment, progress, this->__uid);
	}
}

void
ImageAttachmentOperation::Perform() {
	__IsRunning = true;
	this->GetUploadServer();
}

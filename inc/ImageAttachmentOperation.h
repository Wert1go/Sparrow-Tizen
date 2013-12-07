/*
 * ImageAttachmentOperation.h
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#ifndef IMAGEATTACHMENTOPERATION_H_
#define IMAGEATTACHMENTOPERATION_H_

#include <FBase.h>

#include "IRestRequestListener.h"

class RestResponse;
class Error;
class RestRequestOperation;
class MAttachment;
class IAttachmentListener;

using namespace Tizen::Base;

class ImageAttachmentOperation
 : public Object
 , public IRestRequestListener
{
public:
	ImageAttachmentOperation();
	virtual ~ImageAttachmentOperation();

	void Perform();

	MAttachment *__pAttachment;
	IAttachmentListener *__pAttachmentListener;
	int __uid;

	bool __IsRunning;

private:
	void OnSuccessN(RestResponse *result);
	void OnErrorN(Error *error);
	void OnProgressChanged(int progress);

	void GetUploadServer();
	void UploadImageToUrl(String *url);
	void SavePhotoOnServer(String *photoData, String *server, String *hash);

private:
	RestRequestOperation *__pGetUploadServerOperation;
	RestRequestOperation *__pUploadImageOperation;
	RestRequestOperation *__pSaveImageOnServerOperation;


};

#endif /* IMAGEATTACHMENTOPERATION_H_ */

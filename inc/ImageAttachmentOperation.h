/*
 * ImageAttachmentOperation.h
 *
 *  Created on: Dec 4, 2013
 *      Author: developer
 */

#ifndef IMAGEATTACHMENTOPERATION_H_
#define IMAGEATTACHMENTOPERATION_H_

#include "IRestRequestListener.h"

class RestResponse;
class Error;
class RestRequestOperation;

class ImageAttachmentOperation
 : public IRestRequestListener
{
public:
	ImageAttachmentOperation();
	virtual ~ImageAttachmentOperation();

	void Perform();

private:
	void OnSuccessN(RestResponse *result);
	void OnErrorN(Error *error);

	void GetUploadServer();
	void UploadImageToUrl(String *url);
	void SavePhotoOnServer(String *photoData, String *server, String *hash);

private:
	RestRequestOperation *__pGetUploadServerOperation;
	RestRequestOperation *__pUploadImageOperation;
	RestRequestOperation *__pSaveImageOnServerOperation;

	bool __IsRunning;
};

#endif /* IMAGEATTACHMENTOPERATION_H_ */

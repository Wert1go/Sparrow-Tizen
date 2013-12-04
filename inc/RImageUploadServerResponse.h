/*
 * RImageUploadServerResponse.h
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#ifndef RIMAGEUPLOADSERVERRESPONSE_H_
#define RIMAGEUPLOADSERVERRESPONSE_H_

#include "RestResponse.h"

class RImageUploadServerResponse
 : public RestResponse

{
public:
	RImageUploadServerResponse();
	virtual ~RImageUploadServerResponse();

	String *__pUploadServer;

	String *__pPhotoData;
	String *__pServerData;
	String *__pHashData;
};

#endif /* RIMAGEUPLOADSERVERRESPONSE_H_ */

/*
 * RImageUploadServerDescriptor.h
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#ifndef RIMAGEUPLOADSERVERDESCRIPTOR_H_
#define RIMAGEUPLOADSERVERDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RImageUploadServerDescriptor
 : public ResponseDescriptor
{
public:
	RImageUploadServerDescriptor();
	virtual ~RImageUploadServerDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RIMAGEUPLOADSERVERDESCRIPTOR_H_ */

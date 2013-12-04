/*
 * RImageUploadDataDescriptor.h
 *
 *  Created on: Dec 5, 2013
 *      Author: developer
 */

#ifndef RIMAGEUPLOADDATADESCRIPTOR_H_
#define RIMAGEUPLOADDATADESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RImageUploadDataDescriptor
 : public ResponseDescriptor
 {
public:
	RImageUploadDataDescriptor();
	virtual ~RImageUploadDataDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RIMAGEUPLOADDATADESCRIPTOR_H_ */

/*
 * RDeleteDescriptor.h
 *
 *  Created on: Dec 18, 2013
 *      Author: developer
 */

#ifndef RDELETEDESCRIPTOR_H_
#define RDELETEDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RDeleteDescriptor: public ResponseDescriptor {
public:
	RDeleteDescriptor();
	virtual ~RDeleteDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RDELETEDESCRIPTOR_H_ */

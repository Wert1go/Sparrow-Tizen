/*
 * RContactsDescriptor.h
 *
 *  Created on: Dec 17, 2013
 *      Author: developer
 */

#ifndef RCONTACTSDESCRIPTOR_H_
#define RCONTACTSDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RContactsDescriptor: public ResponseDescriptor {
public:
	RContactsDescriptor();
	virtual ~RContactsDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RCONTACTSDESCRIPTOR_H_ */

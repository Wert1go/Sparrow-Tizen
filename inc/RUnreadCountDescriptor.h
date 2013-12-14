/*
 * RUnreadCountDescriptor.h
 *
 *  Created on: Dec 15, 2013
 *      Author: developer
 */

#ifndef RUNREADCOUNTDESCRIPTOR_H_
#define RUNREADCOUNTDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RUnreadCountDescriptor: public ResponseDescriptor {
public:
	RUnreadCountDescriptor();
	virtual ~RUnreadCountDescriptor();
	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RUNREADCOUNTDESCRIPTOR_H_ */

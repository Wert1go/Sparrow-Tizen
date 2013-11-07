/*
 * UserDescriptor.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef USERDESCRIPTOR_H_
#define USERDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class UserDescriptor: public ResponseDescriptor {
public:
	UserDescriptor();
	virtual ~UserDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* USERDESCRIPTOR_H_ */

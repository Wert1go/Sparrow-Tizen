/*
 * RImageSaveDescriptor.h
 *
 *  Created on: Dec 6, 2013
 *      Author: developer
 */

#ifndef RIMAGESAVEDESCRIPTOR_H_
#define RIMAGESAVEDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RImageSaveDescriptor: public ResponseDescriptor {
public:
	RImageSaveDescriptor();
	virtual ~RImageSaveDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RIMAGESAVEDESCRIPTOR_H_ */

/*
 * RMessageSendExecuteDescriptor.h
 *
 *  Created on: Dec 10, 2013
 *      Author: developer
 */

#ifndef RMESSAGESENDEXECUTEDESCRIPTOR_H_
#define RMESSAGESENDEXECUTEDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RMessageSendExecuteDescriptor
: public ResponseDescriptor {
public:
	RMessageSendExecuteDescriptor();
	virtual ~RMessageSendExecuteDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RMESSAGESENDEXECUTEDESCRIPTOR_H_ */

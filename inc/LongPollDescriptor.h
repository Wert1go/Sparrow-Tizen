/*
 * LongPollDescriptor.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLDESCRIPTOR_H_
#define LONGPOLLDESCRIPTOR_H_

#include "ResponseDescriptor.h"
#include "RestResponse.h"
#include <FWeb.h>

using namespace Tizen::Web::Json;

class LongPollDescriptor
 : public ResponseDescriptor
{
public:
	LongPollDescriptor();
	virtual ~LongPollDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* LONGPOLLDESCRIPTOR_H_ */

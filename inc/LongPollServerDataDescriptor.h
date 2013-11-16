/*
 * LongPollServerDataDescriptor.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLSERVERDATADESCRIPTOR_H_
#define LONGPOLLSERVERDATADESCRIPTOR_H_

#include "ResponseDescriptor.h"
#include "RestResponse.h"
#include <FWeb.h>

using namespace Tizen::Web::Json;

class LongPollServerDataDescriptor
 : public ResponseDescriptor
{
public:
	LongPollServerDataDescriptor();
	virtual ~LongPollServerDataDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* LONGPOLLSERVERDATADESCRIPTOR_H_ */

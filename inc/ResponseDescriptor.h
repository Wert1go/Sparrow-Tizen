/*
 * ResponseDescriptor.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef RESPONSEDESCRIPTOR_H_
#define RESPONSEDESCRIPTOR_H_

#include "RestResponse.h"
#include <FWeb.h>

using namespace Tizen::Web::Json;

class ResponseDescriptor {
public:
	ResponseDescriptor();
	virtual ~ResponseDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject) = 0;

	bool IsAuthorized(JsonObject* pObject);
};

#endif /* RESPONSEDESCRIPTOR_H_ */

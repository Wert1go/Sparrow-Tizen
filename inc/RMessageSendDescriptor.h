/*
 * RMessageSendDescriptor.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef RMESSAGESENDDESCRIPTOR_H_
#define RMESSAGESENDDESCRIPTOR_H_

#include "ResponseDescriptor.h"
#include <FWeb.h>

using namespace Tizen::Web::Json;

class RMessageSendDescriptor
 : public ResponseDescriptor
{
public:
	RMessageSendDescriptor();
	virtual ~RMessageSendDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RMESSAGESENDDESCRIPTOR_H_ */

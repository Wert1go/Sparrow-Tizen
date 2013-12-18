/*
 * RDeleteDescriptor.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: developer
 */

#include "RDeleteDescriptor.h"
#include "RMessageSendResponse.h"

RDeleteDescriptor::RDeleteDescriptor() {
	// TODO Auto-generated constructor stub

}

RDeleteDescriptor::~RDeleteDescriptor() {
	// TODO Auto-generated destructor stub
}

RestResponse *
RDeleteDescriptor::performObjectMappingN(JsonObject* pObject) {
	AppLog("RDeleteDescriptor - 1");
	RMessageSendResponse *response = new RMessageSendResponse();
	response->__mid = 1;
	return response;
}

/*
 * RestResponse.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include <FBase.h>

#include "RestResponse.h"

RestResponse::RestResponse() {
	__error = null;
}

RestResponse::~RestResponse() {
	// TODO Auto-generated destructor stub
}

Error *RestResponse::GetError() {
	return __error;
}

void RestResponse::SetOperationCode(long opCode) {
	__operationCode = opCode;
}

long RestResponse::GetOperationCode() {
	return __operationCode;
}


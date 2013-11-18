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

}

Error *
RestResponse::GetError() {
	return __error;
}

void
RestResponse::SetError(Error *error) {
	if (__error) {
		delete __error;
	}

	__error = error;
}

void RestResponse::SetOperationCode(long opCode) {
	__operationCode = opCode;
}

long RestResponse::GetOperationCode() {
	return __operationCode;
}


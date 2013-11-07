/*
 * Error.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#include "Error.h"

Error::Error() {
	__code = UNKNOW;
}

Error::Error(ErrorCode code) {
	__code = code;
}

ErrorCode
Error::GetCode() {
	return __code;
}

Error::~Error() {

}


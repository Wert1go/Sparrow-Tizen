/*
 * RestResponse.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef RESTRESPONSE_H_
#define RESTRESPONSE_H_

#include "Error.h"

class RestResponse {
public:
	RestResponse();
	virtual ~RestResponse();

public:
	Error *GetError();
	void SetError(Error *error);

	void SetOperationCode(long opCode);
	long GetOperationCode();



private:
	Error *__error;
	long __operationCode;
};

#endif /* RESTRESPONSE_H_ */

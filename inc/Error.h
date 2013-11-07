/*
 * Error.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef ERROR_H_
#define ERROR_H_

typedef enum {
	UNKNOW,
	REST_BAD_RESPONSE
} ErrorCode;

class Error {
public:
	Error();
	Error(ErrorCode code);
	virtual ~Error();

public:
	ErrorCode GetCode();

private:
	ErrorCode __code;
};

#endif /* ERROR_H_ */

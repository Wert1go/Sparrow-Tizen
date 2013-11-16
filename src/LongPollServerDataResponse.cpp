/*
 * LongPollServerDataResponse.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollServerDataResponse.h"
#include "RestResponse.h"

using namespace Tizen::Base;

LongPollServerDataResponse::LongPollServerDataResponse() {
	RestResponse::RestResponse();

	__pKey = null;
	__pServer = null;
	__pTS = null;
}

LongPollServerDataResponse::~LongPollServerDataResponse() {

}

String *
LongPollServerDataResponse::GetKey() {
	return __pKey;
}

String *
LongPollServerDataResponse::GetServer() {
	return __pServer;
}

String *
LongPollServerDataResponse::GetTS() {
	return __pTS;
}

void
LongPollServerDataResponse::SetKey(String *pKey) {
	if (__pKey) {
		delete __pKey;
	}
	__pKey = pKey;
}

void
LongPollServerDataResponse::SetServer(String *pServer) {
	if (__pServer) {
		delete __pServer;
	}

	__pServer = pServer;
}

void
LongPollServerDataResponse::SetTS(String *pTS) {
	if (__pTS) {
		delete __pTS;
	}

	__pTS = pTS;
}

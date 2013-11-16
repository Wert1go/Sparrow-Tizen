/*
 * LongPollResponse.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollResponse.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

LongPollResponse::LongPollResponse() {
	__pUpdates = null;
	__pTs = null;
}

LongPollResponse::~LongPollResponse() {
	delete __pTs;
	delete __pUpdates;

}

void
LongPollResponse::SetUpdates(LinkedList *updates) {
	if (__pUpdates) {
		delete __pUpdates;
	}

	__pUpdates = updates;
}

LinkedList *
LongPollResponse::GetUpdates() {
	return __pUpdates;
}

void
LongPollResponse::SetTS(String *ts) {
	if (__pTs) {
		delete __pTs;
	}

	__pTs = ts;
}

String *
LongPollResponse::GetTS() {
	return __pTs;
}

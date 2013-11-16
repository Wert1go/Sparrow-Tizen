/*
 * RMessagesResponse.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "RMessagesResponse.h"

RMessagesResponse::RMessagesResponse() {

}

RMessagesResponse::~RMessagesResponse() {

}

void
RMessagesResponse::SetMessages(LinkedList *messages) {
	__pMessages = messages;
}

LinkedList *
RMessagesResponse::GetMessages() {
	return __pMessages;
}

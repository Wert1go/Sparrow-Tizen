/*
 * RDialogResponse.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "RDialogResponse.h"

RDialogResponse::RDialogResponse() {
	// TODO Auto-generated constructor stub
	this->__pDialogs = null;
}

RDialogResponse::~RDialogResponse() {
	// TODO Auto-generated destructor stub
}

LinkedList *
RDialogResponse::GetDialogs() {
	return this->__pDialogs;
}

void
RDialogResponse::SetDialogs(LinkedList * list) {
	if (this->__pDialogs) {
		delete this->__pDialogs;
	}

	this->__pDialogs = list;
}

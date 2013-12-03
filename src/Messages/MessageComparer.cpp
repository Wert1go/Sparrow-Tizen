/*
 * MessageComparer.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#include "MessageComparer.h"

#include <FBase.h>

#include "MMessage.h"

using namespace Tizen::Base;

MessageComparer::MessageComparer() {

}

MessageComparer::~MessageComparer() {

}

result
MessageComparer::Compare(const Tizen::Base::Object& obj1, const Tizen::Base::Object& obj2, int& cmp) const {
	result r = E_SUCCESS;
	 const MMessage *message1 = dynamic_cast<const MMessage *>(&obj1);
	 const MMessage *message2 = dynamic_cast<const MMessage *>(&obj2);

	 if (message1 && message2) {
		if (message1->__date == message2->__date) {
			cmp = 0;
		} else if (message1->__date > message2->__date) {
			cmp = 1;
		} else {
			cmp = -1;
		}
	 } else {
		 r = E_FAILURE;
	 }

	return r;
}


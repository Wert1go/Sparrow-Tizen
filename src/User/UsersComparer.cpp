/*
 * UsersComparer.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: developer
 */

#include "UsersComparer.h"

#include <FBase.h>

using namespace Tizen::Base;
#include "MUser.h"

UsersComparer::UsersComparer() {
	// TODO Auto-generated constructor stub

}

UsersComparer::~UsersComparer() {
	// TODO Auto-generated destructor stub
}

result
UsersComparer::Compare(const Tizen::Base::Object& obj1, const Tizen::Base::Object& obj2, int& cmp) const {
	result r = E_SUCCESS;
	 const MUser *user1 = dynamic_cast<const MUser *>(&obj1);
	 const MUser *user2 = dynamic_cast<const MUser *>(&obj2);

	 if (user1 && user2) {

		 StringComparer comparer;
		 int result;
		 comparer.Compare(*user1->__firstName, *user2->__firstName, result);

		 cmp = result;
	 } else {
		 r = E_FAILURE;
	 }

	return r;
}

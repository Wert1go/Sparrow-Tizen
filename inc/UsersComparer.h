/*
 * UsersComparer.h
 *
 *  Created on: Nov 21, 2013
 *      Author: developer
 */

#ifndef USERSCOMPARER_H_
#define USERSCOMPARER_H_

#include <FBase.h>

using namespace Tizen::Base;

class UsersComparer
: public Tizen::Base::Collection::IComparer
, public Object {
public:
	UsersComparer();
	virtual ~UsersComparer();

	virtual result Compare(const Tizen::Base::Object& obj1, const Tizen::Base::Object& obj2, int& cmp) const;
};

#endif /* USERSCOMPARER_H_ */

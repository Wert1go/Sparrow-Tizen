/*
 * MessageComparer.h
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#ifndef MESSAGECOMPARER_H_
#define MESSAGECOMPARER_H_

#include <FBaseColIComparer.h>

using namespace Tizen::Base;

class MessageComparer
: public Tizen::Base::Collection::IComparer
, public Object
  {
public:
	MessageComparer();
	virtual ~MessageComparer();

	virtual result Compare(const Tizen::Base::Object& obj1, const Tizen::Base::Object& obj2, int& cmp) const;
};

#endif /* MESSAGECOMPARER_H_ */

/*
 * UpdateUnit.h
 *
 *  Created on: Nov 11, 2013
 *      Author: developer
 */

#ifndef UPDATEUNIT_H_
#define UPDATEUNIT_H_

#include <FBase.h>

using namespace Tizen::Base;

class UpdateUnit : public Object {
public:
	UpdateUnit();
	virtual ~UpdateUnit();

	int __index;
	int __section;
	int __requestId;
};

#endif /* UPDATEUNIT_H_ */

/*
 * MDialogsDescriptor.h
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#ifndef MDIALOGSDESCRIPTOR_H_
#define MDIALOGSDESCRIPTOR_H_

#include "ResponseDescriptor.h"

using namespace Tizen::Base::Collection;

class MDialogsDescriptor
 : public ResponseDescriptor
{
public:
	MDialogsDescriptor(bool persist = true);
	virtual ~MDialogsDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
private:
	bool __persist;
};

#endif /* MDIALOGSDESCRIPTOR_H_ */

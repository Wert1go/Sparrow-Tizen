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
	MDialogsDescriptor();
	virtual ~MDialogsDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* MDIALOGSDESCRIPTOR_H_ */

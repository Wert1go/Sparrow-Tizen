/*
 * RSearchDialogDescriptor.h
 *
 *  Created on: Dec 15, 2013
 *      Author: developer
 */

#ifndef RSEARCHDIALOGDESCRIPTOR_H_
#define RSEARCHDIALOGDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RestResponse;

class RSearchDialogDescriptor
 : public ResponseDescriptor
{
public:
	RSearchDialogDescriptor();
	virtual ~RSearchDialogDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
};

#endif /* RSEARCHDIALOGDESCRIPTOR_H_ */

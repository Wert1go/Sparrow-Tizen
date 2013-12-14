/*
 * MMessageDescriptor.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef MMESSAGEDESCRIPTOR_H_
#define MMESSAGEDESCRIPTOR_H_

#include "ResponseDescriptor.h"

using namespace Tizen::Base::Collection;

class MMessageDescriptor
 : public ResponseDescriptor
{
public:
	MMessageDescriptor();
	virtual ~MMessageDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);
	void LoadUsers(IList * pFwdMessages, IList *pUsers);
};

#endif /* MMESSAGEDESCRIPTOR_H_ */

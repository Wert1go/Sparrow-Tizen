/*
 * RFriendsDescriptor.h
 *
 *  Created on: Nov 21, 2013
 *      Author: developer
 */

#ifndef RFRIENDSDESCRIPTOR_H_
#define RFRIENDSDESCRIPTOR_H_

#include "ResponseDescriptor.h"

class RFriendsDescriptor
: public ResponseDescriptor
{
public:
	RFriendsDescriptor();
	virtual ~RFriendsDescriptor();

	virtual RestResponse *performObjectMappingN(JsonObject* pObject);

	void SetPersisterActive(bool state);
private:
	bool __isPresisterActive;
};

#endif /* RFRIENDSDESCRIPTOR_H_ */

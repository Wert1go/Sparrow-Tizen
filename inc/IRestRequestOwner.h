/*
 * IRestRequestOwner.h
 *
 *  Created on: Nov 7, 2013
 *      Author: developer
 */

#ifndef IRESTREQUESTOWNER_H_
#define IRESTREQUESTOWNER_H_

#include <FBase.h>
#include "RestRequestOperation.h"

class IRestRequestOwner {
public:
	virtual ~IRestRequestOwner(void){};

	virtual void OnCompliteN(void *operation) = 0;
};

#endif /* IRESTREQUESTOWNER_H_ */

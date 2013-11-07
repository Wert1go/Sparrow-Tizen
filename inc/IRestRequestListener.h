/*
 * IRestRequestListener.h
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#ifndef IRESTREQUESTLISTENER_H_
#define IRESTREQUESTLISTENER_H_

#include "RestResponse.h"
#include "Error.h"

class IRestRequestListener {
public:
	virtual ~IRestRequestListener(void){};

	virtual void OnSuccessN(RestResponse *result){};
	virtual void OnErrorN(Error *error){};
};

#endif /* IRESTREQUESTLISTENER_H_ */

/*
 * RDialogResponse.h
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#ifndef RDIALOGRESPONSE_H_
#define RDIALOGRESPONSE_H_

#import "RestResponse.h"
#import "MDialog.h"

using namespace Tizen::Base::Collection;

class RDialogResponse
 : public RestResponse
{
public:
	RDialogResponse();
	virtual ~RDialogResponse();

	LinkedList * GetDialogs();
	void SetDialogs(LinkedList * list);

private:

	LinkedList *__pDialogs;
};

#endif /* RDIALOGRESPONSE_H_ */

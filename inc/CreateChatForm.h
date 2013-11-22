/*
 * CreateChatForm.h
 *
 *  Created on: Nov 23, 2013
 *      Author: developer
 */

#ifndef CREATECHATFORM_H_
#define CREATECHATFORM_H_

#include <FUiCtrlForm.h>
#include "BaseUserController.h"

class CreateChatForm
 : public Tizen::Ui::Controls::Form
 , public BaseUserController
{
public:
	CreateChatForm();
	virtual ~CreateChatForm();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	 virtual void RequestUiUpdate(int code, IList *pArgs);

};

#endif /* CREATECHATFORM_H_ */

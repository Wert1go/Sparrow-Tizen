/*
 * AuthForm.h
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#ifndef AUTHFORM_H_
#define AUTHFORM_H_

#include <FUi.h>

class AuthForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener {

public:
	AuthForm();
	virtual ~AuthForm();
	virtual result OnDraw(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
	Tizen::Ui::Controls::Button* pAuthButton;
	Tizen::Ui::Controls::Button* pRegButton;
	Tizen::Ui::Controls::TextBox* pInfoLabel;
};

#endif /* AUTHFORM_H_ */

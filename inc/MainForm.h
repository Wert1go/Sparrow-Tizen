/*
 * MainForm.h
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#ifndef MAINFORM_H_
#define MAINFORM_H_

#include <FUi.h>

class MainForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Ui::IActionEventListener

 {
public:
	MainForm();
	virtual ~MainForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
};

#endif /* MAINFORM_H_ */

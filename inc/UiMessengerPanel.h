/*
 * UiMessengerPanel.h
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#ifndef UIMESSENGERPANEL_H_
#define UIMESSENGERPANEL_H_

#include <FUi.h>
#include <FBase.h>

using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;

class UiMessengerPanel: public Tizen::Ui::Controls::Panel {
public:
	UiMessengerPanel();
	virtual ~UiMessengerPanel();

	ExpandableEditArea* GetEditArea();
	Button* GetSendButton();
	Button* GetAttachButton();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	void SetRectangle(FloatRectangle rect);

private:
	ExpandableEditArea* __pEditArea;
	Button *__pSendButton;
	Button *__pAttachButton;

};

#endif /* UIMESSENGERPANEL_H_ */

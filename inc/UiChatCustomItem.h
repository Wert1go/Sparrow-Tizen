/*
 * UiChatCustomItem.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef UICHATCUSTOMITEM_H_
#define UICHATCUSTOMITEM_H_

#include <FUi.h>

using namespace Tizen::Ui;
using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

class UiChatListItem;
class MMessage;
class MDialog;

class UiChatCustomItem
 : public CustomItem
{
public:
	UiChatCustomItem();
	virtual ~UiChatCustomItem();

	void Init();
	void SetDimension(Dimension *pDimension);
	void SetBubbleDimension(Dimension pDimension);
	void SetMessage(MMessage *pMessage);
	MMessage *GetMessage();

	void SetDialog(MDialog *pDialog);
	MDialog *GetDialog();

private:
	Dimension *__pDimension;
	Dimension __pBubbleDimension;

	static const int ID_USER_MESSAGE = 105;

	UiChatListItem* __pChatListItem;
	MMessage *__pMessage;
	MDialog *__pDialog;
};

#endif /* UICHATCUSTOMITEM_H_ */

/*
 * UiUserListItem.h
 *
 *  Created on: Nov 20, 2013
 *      Author: developer
 */

#ifndef UIUSERLISTITEM_H_
#define UIUSERLISTITEM_H_

#include <FUi.h>
#include <FGraphics.h>

using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class MUser;

class UiUserListItem
 : public ICustomElement
{
public:
	UiUserListItem();
	virtual ~UiUserListItem();

	virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

	void SetUser(MUser *pUser);
	MUser *GetUser();
private:
	MUser *__pUser;
	EnrichedText* __pUserName;
	TextElement* __pUserNameText;

	EnrichedText* __pContactName;
	TextElement* __pContactText;

};

#endif /* UIUSERLISTITEM_H_ */

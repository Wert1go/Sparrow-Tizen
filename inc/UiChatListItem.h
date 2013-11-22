/*
 * UiChatListItem.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef UICHATLISTITEM_H_
#define UICHATLISTITEM_H_

#include <FUiCtrlICustomElement.h>

using namespace Tizen::Graphics;

class MDialog;
class MMessage;
class MUser;
class IImageDrawer;

class UiChatListItem
 : public Tizen::Ui::Controls::ICustomElement {
public:
	UiChatListItem();
	virtual ~UiChatListItem();

	virtual bool OnDraw(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

	void SetMessage(MMessage *pMessage);
	MMessage *GetMessage();
	void SetBubbleDimension(Dimension pDimension);

	void SetDialog(MDialog *pDialog);
	MDialog *GetDialog();

	void SetDrawer(IImageDrawer *drawer);
	IImageDrawer * GetDrawer();

private:
	void DrawBubble(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status);
	void DrawMessage(Tizen::Graphics::Canvas& canvas, const Tizen::Graphics::Rectangle& rect, Tizen::Ui::Controls::ListItemDrawingStatus status);
private:
	MMessage *__pMessage;
	Dimension __pBubbleDimension;
	float __offset;
	float __sideOffset;
	float __leftOffset;

	float __triangleWidth;
	float __triangleHeight;
	MDialog *__pDialog;
	MUser *__pUser;

	IImageDrawer *__pImageDrawer;

};

#endif /* UICHATLISTITEM_H_ */

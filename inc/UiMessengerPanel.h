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
#include <FGraphics.h>
#include "IAttachmentItemListener.h"

using namespace Tizen::Graphics;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

class AttachmentItem;
class MAttachment;
class IRequestAttachmentDelete;

extern const int editAreaOffset;

class UiMessengerPanel
 : public Tizen::Ui::Controls::Panel
 , public ITouchEventListener
 , public IAttachmentItemListener
 , public Tizen::Ui::IActionEventListener

 {

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

	void AddAttachment(MAttachment *attachment);
	void RemoveAttachment(MAttachment *attachment, int index);

	IRequestAttachmentDelete *__pAttachmentOwner;
	LinkedList *__pItems;

protected:
	bool	__bButtonMoved;
	Point	__PointLastTouch;
	Panel *__pScrollPanel;



	int GetCount();
	void ReloadData();
	void UpdateScrollPanel();

	AttachmentItem *CreateItem(int index, MAttachment *attach);

	Button *__pAddButton;

private:
	ExpandableEditArea* __pEditArea;
	Button *__pSendButton;
	Button *__pAttachButton;

private:
	//-- ITouchEventListener
	virtual void OnTouchPressed(
			const Tizen::Ui::Control &source,
			const Tizen::Graphics::Point &currentPosition,
			const Tizen::Ui::TouchEventInfo &touchInfo);

	virtual void OnTouchMoved(
			const Tizen::Ui::Control &source,
			const Tizen::Graphics::Point &currentPosition,
			const Tizen::Ui::TouchEventInfo &touchInfo);
	//empty
	virtual void OnTouchLongPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)	{};
	virtual void OnTouchReleased(const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)		{};
	virtual void OnTouchDoublePressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo) 	{};
	virtual void OnTouchFocusIn(const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)		{};
	virtual void OnTouchFocusOut(const Tizen::Ui::Control &source, const Tizen::Graphics::Point &currentPosition, const Tizen::Ui::TouchEventInfo &touchInfo)		{};

	//
	virtual void DeleteButtonPressedAtIndex(int index);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

public:
	void SetAttachmentReady(bool state, int index, MAttachment *attachment);
	void UpdateAttachmentProgress(int progress, int index, MAttachment *attachment);
	void ClearAttachments();
};

#endif /* UIMESSENGERPANEL_H_ */

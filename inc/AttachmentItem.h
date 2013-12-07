/*
 * AttachmentItem.h
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#ifndef ATTACHMENTITEM_H_
#define ATTACHMENTITEM_H_

#include <FUiCtrlPanel.h>
#include <FUi.h>

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

class IAttachmentItemListener;
class MAttachment;

class AttachmentItem
 : public Tizen::Ui::Controls::Panel
 , public Tizen::Ui::IActionEventListener
{
public:
	AttachmentItem();
	virtual ~AttachmentItem();

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	result SetCustomBounds(const Tizen::Graphics::Rectangle& rect);

	Bitmap *__pIcon;
	Bitmap *__pImage;

	EnrichedText* __pTitleLabel;
	TextElement* __pTitleText;

	Button *__pButton;
	int __progress;

	virtual result OnDraw(void);

	int __index;
	bool __ready;

	IAttachmentItemListener *__pItemEventListener;

	MAttachment *__pAttachment;

	void SetReady(bool state);
	void UpdateProgressValue(int progress);

private:
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
};

#endif /* ATTACHMENTITEM_H_ */

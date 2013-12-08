/*
 * UiAttachmentListPopup.h
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#ifndef UIATTACHMENTLISTPOPUP_H_
#define UIATTACHMENTLISTPOPUP_H_

#include <FUi.h>

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;

class IPopupHandler;

class UiAttachmentListPopup
 : public Tizen::Ui::Controls::Popup
 , public Tizen::Ui::IPropagatedKeyEventListener
 , public Tizen::Ui::Controls::IListViewItemEventListener
 , public Tizen::Ui::Controls::IListViewItemProvider
   {
public:
	UiAttachmentListPopup();
	virtual ~UiAttachmentListPopup();

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	void ShowPopup(void);
	void HidePopup(void);

	virtual bool OnKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool TranslateKeyEventInfo(Tizen::Ui::Control& source, Tizen::Ui::KeyEventInfo& keyEventInfo);

	 // IListViewItemEventListener
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView &listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView &listView, int index, Tizen::Ui::Controls::SweepDirection direction);

		// IListViewItemProvider
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual int GetItemCount(void);

	IPopupHandler *__pPopupHandler;

private:
	Tizen::Ui::Controls::ListView *__pListView;
	Tizen::Ui::Controls::ListContextItem* __pItemContext;

	Bitmap *pPhotoIcon;
	Bitmap *pAudioIcon;
	Bitmap *pVideoIcon;
	Bitmap *pDocIcon;
	Bitmap *pMapIcon;

};

#endif /* UIATTACHMENTLISTPOPUP_H_ */

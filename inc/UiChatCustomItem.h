/*
 * UiChatCustomItem.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef UICHATCUSTOMITEM_H_
#define UICHATCUSTOMITEM_H_

#include <FUi.h>
#include "IImageDrawer.h"
#include "IImageLoadingListener.h"

using namespace Tizen::Ui;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

class UiChatListItem;
class MMessage;
class MDialog;
class IRefreshableListView;
class Error;
class MAttachment;

class UiChatCustomItem
 : public CustomItem
 , public IImageLoadingListener
 , public IImageDrawer
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
	void SetIndex(int index);

	virtual void AddRefreshListener(IRefreshableListView *pRefreshListener);

private:
	virtual void DrawImageFromUrlInRect(String *imageUrl, Rectangle rect);
	virtual void DrawAttachmentFromUrlInRect(String *imageUrl, Rectangle rect, MAttachment *attachment);

	void OnImageLoadedN(Bitmap *result, Integer *code);
	void OnErrorN(Error *error);

	void SetImageUrl(String *url, int code);

private:
	Dimension *__pDimension;
	Dimension __pBubbleDimension;

	static const int ID_USER_MESSAGE = 105;

	UiChatListItem* __pChatListItem;
	MMessage *__pMessage;
	MDialog *__pDialog;

	int __index;
	int __section;

	HashMapT<String *, Integer *> *__pUrtToIndexMap;
	LinkedList *__pImageViews;
	IRefreshableListView *__pRefreshListener;

};

#endif /* UICHATCUSTOMITEM_H_ */

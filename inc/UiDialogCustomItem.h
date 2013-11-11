/*
 * UiDialogCustomItem.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef UIDIALOGCUSTOMITEM_H_
#define UIDIALOGCUSTOMITEM_H_

#include <FUi.h>
#include "IRefreshableListView.h"
#include "IImageLoadingListener.h"
#include "Error.h"

using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

class UiDialogCustomItem
 : public CustomItem
 , public IImageLoadingListener
   {
public:
	UiDialogCustomItem();
	virtual ~UiDialogCustomItem (void);

	//virtual bool OnDraw (Tizen::Graphics::Canvas &canvas, const Tizen::Graphics::Rectangle &rect, Tizen::Ui::Controls::ListItemDrawingStatus status);

	void Init();
	void SetImageUrl(String *url);

	void SetIndex(int index);
	int GetIndex();
	void AddRefreshListener(IRefreshableListView *pRefreshListener);

	static const int ID_USER_AVATAR = 104;
	static const int ID_SECOND_USER_AVATAR = 109;

private:
	virtual void OnImageLoadedN(Bitmap *result);
	virtual void OnErrorN(Error *error);

private:
	String *__pUrl;
	Bitmap *__pPlaceholder;
	Bitmap *PlaceholderActive;
	Bitmap *__pRounder;
	Bitmap *__pDialogIcon;

	static const int ID_USER_AVATAR_MASK = 105;
	static const int ID_USER_MESSAGE = 106;
	static const int ID_SECOND_USER_AVATAR_MASK = 107;
	static const int ID_SECOND_USER_MESSAGE = 108;

	int __index;
	IRefreshableListView *__pRefreshListener;
};

#endif /* UIDIALOGCUSTOMITEM_H_ */

/*
 * UiChapPanel.h
 *
 *  Created on: Nov 19, 2013
 *      Author: developer
 */

#ifndef UICHAPPANEL_H_
#define UICHAPPANEL_H_

#include <FUiCtrlPanel.h>
#include "IImageLoadingListener.h"

using namespace Tizen::Base;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

class MUser;
class Error;

class UiChapPanel
 : public Tizen::Ui::Controls::Panel
 , public IImageLoadingListener
   {
public:
	UiChapPanel();
	virtual ~UiChapPanel();

	void SetIsOnline(bool online);
	void SetUser(MUser *user);
	bool Initialize(void);
	//void SetChat();
private:
	virtual result OnDraw(void);
	virtual void OnImageLoadedN(Bitmap *result);
	virtual void OnErrorN(Error *error);
	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

private:
	bool __IsOinline;

	MUser *__pUser;

	Bitmap *__pRounder;
	Bitmap *__pOnlineIcon;
	Bitmap *__ChatIcon;

	String *titleMessage;
	String *descriptionMessage;
};

#endif /* UICHAPPANEL_H_ */

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

using namespace Tizen::Base::Runtime;

class MDialog;
class Error;

class UiChapPanel
 : public Tizen::Ui::Controls::Panel
 , public IImageLoadingListener
 , public ITimerEventListener
   {
public:
	UiChapPanel();
	virtual ~UiChapPanel();

	void SetIsOnline(bool online);
	void SetDialog(MDialog *dialog);
	bool Initialize(void);

	void SetUserPrinting(int userId);
	//void SetChat();
private:
	virtual result OnDraw(void);
	virtual void OnImageLoadedN(Bitmap *result, Integer *code);
	virtual void OnErrorN(Error *error);
	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	virtual void OnTimerExpired (Timer &timer);

private:
	bool __IsOinline;

	MDialog *__pDialog;

	Bitmap *__pRounder;
	Bitmap *__pOnlineIcon;
	Bitmap *__ChatIcon;

	String *titleMessage;
	String *descriptionMessage;

	String *__pPrintingMessage;

	Timer *__pPrintingTimer;
	bool __isUserPrinting;
};

#endif /* UICHAPPANEL_H_ */

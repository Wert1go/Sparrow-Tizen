/*
 * IPopupHandler.h
 *
 *  Created on: Dec 7, 2013
 *      Author: developer
 */

#ifndef IPOPUPHANDLER_H_
#define IPOPUPHANDLER_H_


class IPopupHandler {
public:
	virtual ~IPopupHandler(void){};

	virtual void DidSelectItemInPopup(int itemIndex, int popupId) = 0;
};

#endif /* IPOPUPHANDLER_H_ */

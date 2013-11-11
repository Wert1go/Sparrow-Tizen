/*
 * IRefreshableListView.h
 *
 *  Created on: Nov 11, 2013
 *      Author: developer
 */

#ifndef IREFRESHABLELISTVIEW_H_
#define IREFRESHABLELISTVIEW_H_


class IRefreshableListView {
public:
	virtual ~IRefreshableListView(void){};

	virtual void RequestUpdateForIndex(int index, int elementId) = 0;
};


#endif /* IREFRESHABLELISTVIEW_H_ */

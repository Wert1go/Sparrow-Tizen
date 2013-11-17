/*
 * IMessageOwner.h
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#ifndef IMESSAGEOWNER_H_
#define IMESSAGEOWNER_H_

class PostMessageOperation;

class IMessageOwner {
public:
	virtual ~IMessageOwner(void){};

	virtual void OnCompliteN(PostMessageOperation *operation) = 0;
};

#endif /* IMESSAGEOWNER_H_ */

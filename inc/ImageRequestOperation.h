/*
 * ImageRequestOperation.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef IMAGEREQUESTOPERATION_H_
#define IMAGEREQUESTOPERATION_H_

#include <FNet.h>
#import "IRequestOperation.h"
#include "IImageRequestListener.h"
#include "IRestRequestOwner.h"

class ImageRequestOperation
 : public Tizen::Net::Http::IHttpTransactionEventListener
 , IRequestOperation
{
public:
	ImageRequestOperation(const Tizen::Base::String *url);
	virtual ~ImageRequestOperation();

private:
	// IHttpTransactionEventListener
	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool authRequired);
	virtual void OnTransactionCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

public:
	void SetRequestOwner(IRestRequestOwner *owner);
	void AddImageRequestListener(IImageRequestListener *pImageRequestListener);

private:
	Tizen::Net::Http::HttpTransaction* __pHttpTransaction;
	Tizen::Base::String *__pUrl;
	IRestRequestOwner *__pRequestOwner;
	IImageRequestListener *__pImageRequestListener;
	bool __isComplited;

	void CheckCompletionAndCleanUp();

public:
	void perform();
	bool GetIsComplited();
};

#endif /* IMAGEREQUESTOPERATION_H_ */

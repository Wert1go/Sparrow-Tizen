/*
 * RestRequestOperation.h
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#ifndef RESTREQUESTOPERATION_H_
#define RESTREQUESTOPERATION_H_

static const long GET_USER = 100999101;

#include <FNet.h>
#include "User.h"
#include "dispatch/dispatch.h"
#include "IRestRequestListener.h"
#include "IRestRequestOwner.h"
#include "ResponseDescriptor.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class RestRequestOperation
 : public Tizen::Net::Http::IHttpTransactionEventListener
{
public:
	RestRequestOperation(long operationCode, String *method, HashMap *params);
	virtual ~RestRequestOperation();

private:
	// IHttpTransactionEventListener
	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool authRequired);
	virtual void OnTransactionCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

public:
	void AddEventListener(IRestRequestListener *listener);
	void SetResponseDescriptor(ResponseDescriptor *responseDescriptor);
	void SetRequestOwner(IRestRequestOwner *owner);

private:
	Tizen::Net::Http::HttpTransaction* __pHttpTransaction;

	IRestRequestListener *__restRequestListener;
	ResponseDescriptor *__responseDescriptor;
	IRestRequestOwner *__requestOwner;
	bool __isComplited;

	String *__method;
	long __operationCode;

	void CheckCompletionAndCleanUp();

public:
	void perform();
	long GetOperationCode();

};

#endif /* RESTREQUESTOPERATION_H_ */

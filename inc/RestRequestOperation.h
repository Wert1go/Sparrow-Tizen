/*
 * RestRequestOperation.h
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#ifndef RESTREQUESTOPERATION_H_
#define RESTREQUESTOPERATION_H_

static const long GET_USER = 100999101;
static const long GET_DIALOGS_EXECUTE = 100999102;

#include <FNet.h>

#include "IRestRequestListener.h"
#include "ResponseDescriptor.h"
#include "IRequestOperation.h"
#include "IRestRequestOwner.h"


using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class RestRequestOperation
 : public Tizen::Net::Http::IHttpTransactionEventListener
 , IRequestOperation
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
	void SetRequestOwner(IRestRequestOwner *owner);
	void AddEventListener(IRestRequestListener *listener);
	void SetResponseDescriptor(ResponseDescriptor *responseDescriptor);


private:
	Tizen::Net::Http::HttpTransaction* __pHttpTransaction;

	IRestRequestListener *__restRequestListener;
	ResponseDescriptor *__responseDescriptor;
	IRestRequestOwner *__pRequestOwner;
	bool __isComplited;
	bool __isError;

	String *__method;
	long __operationCode;

	ByteBuffer *__pByteBuffer;

public:
	void perform();
	long GetOperationCode();
	bool GetIsComplited();
	void Execute();

};

#endif /* RESTREQUESTOPERATION_H_ */

/*
 * RestRequestOperation.h
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#ifndef RESTREQUESTOPERATION_H_
#define RESTREQUESTOPERATION_H_

static const long GET_USER = 			100999101;
static const long GET_DIALOGS_EXECUTE = 100999102;

static const long LONGPOLL_GET_SERVER = 100999103;
static const long LONGPOLL_CONNECTION = 100999104;

static const long GET_MESSAGES_HISTORY = 100999105;
static const long GET_MESSAGES_HISTORY_BACKWARD = 100999106;

static const long SEND_MESSAGE = 100999107;

static const long MARK_AS_READ = 100999108;
static const long GET_FRIENDS = 100999109;

static const long CREATE_CHAT = 100999110;

static const long GET_FRIENDS_REQUEST = 100999111;
static const long GET_FRIENDS_SUGGESST = 100999112;
static const long SEARCH_USERS = 100999113;

static const long ADD_TO_FRIEND_REQUEST = 100999114;

static const long ATTACHMENT_IMAGE_GET_SERVER = 100999115;
static const long ATTACHMENT_IMAGE_UPLOAD = 100999116;
static const long ATTACHMENT_IMAGE_SAVE = 100999117;

static const long LONGPOLL_RECONNECTION = 100999118;




static const long NOTIFY_USER_PRINTING = 100999118;

extern const wchar_t* USER_FILEDS;

#include <FNet.h>

#include "IRestRequestListener.h"
#include "ResponseDescriptor.h"
#include "IRequestOperation.h"
#include "IRestRequestOwner.h"


using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

class RestRequestOperation
 : public Tizen::Net::Http::IHttpTransactionEventListener
 , public Tizen::Net::Http::IHttpProgressEventListener
 , IRequestOperation
{
public:
	RestRequestOperation(String *uri, long operationCode, String *method, HashMap *params, HashMap *files);
	RestRequestOperation(String *uri, long operationCode, String *method, HashMap *params);
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


	// IHttpProgressEventListener handlers are declared
	virtual void OnHttpUploadInProgress(Tizen::Net::Http::HttpSession& httpSession,
									  Tizen::Net::Http::HttpTransaction& httpTransaction,
									  long long currentLength,
									  long long totalLength);

	virtual void OnHttpDownloadInProgress(Tizen::Net::Http::HttpSession& httpSession,
								Tizen::Net::Http::HttpTransaction& httpTransaction, long long currentLength, long long totalLength){};


	void Init(String *_uri, long operationCode, String *method, HashMap *params, HashMap *files);

public:
	void SetRequestOwner(IRestRequestOwner *owner);
	void AddEventListener(IRestRequestListener *listener);
	void SetResponseDescriptor(ResponseDescriptor *responseDescriptor);

	bool __isSync;

private:
	Tizen::Net::Http::HttpTransaction* __pHttpTransaction;
	Tizen::Net::Http::HttpSession* __pHttpSession;

	IRestRequestListener *__restRequestListener;
	ResponseDescriptor *__responseDescriptor;
	IRestRequestOwner *__pRequestOwner;
	bool __isComplited;
	bool __isError;

	String *__pRequestUrl;
	HashMap *__pFiles;
	String *__method;
	long __operationCode;

	ByteBuffer *__pByteBuffer;

	void CreateHttpRequest();

public:
	void perform();
	long GetOperationCode();
	bool GetIsComplited();
	void Execute();

};

#endif /* RESTREQUESTOPERATION_H_ */

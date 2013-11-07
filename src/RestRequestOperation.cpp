/*
 * RestRequestOperation.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#include "RestRequestOperation.h"
#include "RestClient.h"
#include "RestResponse.h"
#include "dispatch/dispatch.h"

#include "Error.h"

#include <FNet.h>
#include <FWeb.h>

using namespace Tizen::Base;
using namespace Tizen::Net::Http;
using namespace Tizen::Web::Json;

RestRequestOperation::RestRequestOperation(long operationCode, String *method, HashMap *params) {

	__operationCode = operationCode;
	__method = method;

	String uri = L"https://api.vk.com/method/";

	uri.Append(method->GetPointer());
	uri.Append(L"?");

	AppLogDebug("Begin!");

	IMapEnumerator* pMapEnum = params->GetMapEnumeratorN();
	String* pKey = null;
	String* pValue = null;
	int index = 0;
	while (pMapEnum->MoveNext() == E_SUCCESS)
	{
		if (index != 0) {
			uri.Append(L"&");
		}

		pKey = static_cast< String* > (pMapEnum->GetKey());
		pValue = static_cast< String* > (pMapEnum->GetValue());

		uri.Append(pKey->GetPointer());
		uri.Append(L"=");
		uri.Append(pValue->GetPointer());
		index++;
	}

	uri.Append(L"&v=5.3");

	AppLogDebug("%S", uri.GetPointer());

	delete pMapEnum;
	delete params;

	//users.get?user_ids=30143161

	HttpHeader* pHeader = null;

	__pHttpTransaction = RestClient::getInstance().GetActiveSession()->OpenTransactionN();
	__pHttpTransaction->AddHttpTransactionListener(*this);

	HttpRequest* pHttpRequest = __pHttpTransaction->GetRequest();

	pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	pHttpRequest->SetUri(uri);
	pHeader = pHttpRequest->GetHeader();
	pHeader->AddField(L"Accept", L"application/json");

	__restRequestListener = null;
	__responseDescriptor = null;
	__requestOwner = null;

	__isComplited = false;
}

RestRequestOperation::~RestRequestOperation() {
	delete __method;
	__method = null;
}

void RestRequestOperation::perform() {
	if (__pHttpTransaction != null) {
		__pHttpTransaction->Submit();
	} else {
		AppLogDebug("Ошибка при попытке выполнить HTTP запрос");
	}
}

long RestRequestOperation::GetOperationCode() {
	return __operationCode;
}

//=========================================================

void RestRequestOperation::AddEventListener(IRestRequestListener *listener) {
	__restRequestListener = listener;
}

void RestRequestOperation::SetRequestOwner(IRestRequestOwner *owner) {
	__requestOwner = owner;
}

//=========================================================

void
RestRequestOperation::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("OnTransactionReadyToRead");
	dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if (pHttpHeader != null)
		{
			String* tempHeaderString = pHttpHeader->GetRawHeaderN();
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			String *text = new String ((const char*)(pBuffer->GetPointer()));

			AppLogDebug("text %S", text->GetPointer());

			delete text;

			IJsonValue* pJson = JsonParser::ParseN(*pBuffer);
			JsonObject* pObject = static_cast< JsonObject* >(pJson);

			RestResponse *response = null;

			if (__responseDescriptor) {
				response = __responseDescriptor->performObjectMappingN(pObject);
				response->SetOperationCode(__operationCode);
			} else {
				AppLogDebug("Вы не предоставили дескриптор для запроса!");
			}

			if (__restRequestListener) {
				if (response) {
					if (response->GetError()) {
						__restRequestListener->OnErrorN(response->GetError());
					} else {
						__restRequestListener->OnSuccessN(response);
					}
				} else {
					__restRequestListener->OnErrorN(new Error());
				}
			} else {
				CheckCompletionAndCleanUp();
			}

			delete pJson;
			delete tempHeaderString;
			delete pBuffer;
		} else {
			if (__restRequestListener) {
				__restRequestListener->OnErrorN(new Error());
			} else {
				CheckCompletionAndCleanUp();
			}
		}
	} else {
		if (__restRequestListener) {
			__restRequestListener->OnErrorN(new Error(REST_BAD_RESPONSE));
		} else {
			CheckCompletionAndCleanUp();
		}
	}

	});
}

void
RestRequestOperation::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted(%s)", GetErrorMessage(r));
	delete &httpTransaction;
	CheckCompletionAndCleanUp();
}

void
RestRequestOperation::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite");
}

void
RestRequestOperation::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool authRequired)
{
	AppLog("OnTransactionHeaderCompleted");
}

void
RestRequestOperation::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("OnTransactionCompleted");

	delete &httpTransaction;
	CheckCompletionAndCleanUp();
}

void
RestRequestOperation::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{
	AppLog("OnTransactionCertVerificationRequiredN");

	httpTransaction.Resume();

	delete pCert;
}

void RestRequestOperation::SetResponseDescriptor(ResponseDescriptor *responseDescriptor) {
	__responseDescriptor = responseDescriptor;
}

void RestRequestOperation::CheckCompletionAndCleanUp() {
	if (__isComplited) {
		__requestOwner->OnCompliteN(this);
	} else {
		__isComplited = true;
	}
}


/*
 * RestClient.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#include "RestClient.h"
#include "dispatch/dispatch.h"

using namespace Tizen::Net::Http;
using namespace Tizen::Base;

RestClient::RestClient() {
}

RestClient::~RestClient() {
	delete __baseUrl;
	delete __pHttpSession;
}

void RestClient::SetBaseUrl(Tizen::Base::String *baseUrl) {
	__baseUrl = baseUrl;
	__pHttpSession = new HttpSession();
	__pHttpSession->Construct(NET_HTTP_SESSION_MODE_MULTIPLE_HOST, null, GetBaseUrl()->GetPointer(), null);
}

Tizen::Base::String * RestClient::GetBaseUrl() {
	return __baseUrl;
}

void RestClient::PerformOperation(RestRequestOperation *operation) {
	operation->SetRequestOwner(this);
	operation->perform();
}

void RestClient::PerformOperation(ImageRequestOperation *operation) {
	operation->SetRequestOwner(this);
	operation->perform();
}

void RestClient::OnCompliteN(IRequestOperation *operation) {
	delete operation;
	operation = null;
}


Tizen::Net::Http::HttpSession* RestClient::GetActiveSession() {
	return __pHttpSession;
}

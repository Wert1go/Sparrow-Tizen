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
	// TODO Auto-generated constructor stub
	__queue = dispatch_get_current_queue();
}

RestClient::~RestClient() {
	// TODO Auto-generated destructor stub
}

void RestClient::SetBaseUrl(Tizen::Base::String *baseUrl) {
	__baseUrl = baseUrl;
	__pHttpSession = new HttpSession();
	__pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, GetBaseUrl()->GetPointer(), null);
}

Tizen::Base::String * RestClient::GetBaseUrl() {
	return __baseUrl;
}

void RestClient::SetDefaultQueue(dispatch_queue_t queue) {
	__queue = queue;
}

void RestClient::PerformOperation(RestRequestOperation *operation) {
	operation->SetRequestOwner(this);
	operation->perform();
}

void RestClient::OnCompliteN(void *operation) {
	delete operation;
	operation = null;

}

Tizen::Net::Http::HttpSession* RestClient::GetActiveSession() {
	return __pHttpSession;
}

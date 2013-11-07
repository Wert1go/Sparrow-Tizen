/*
 * RestClient.h
 *
 *  Created on: Nov 6, 2013
 *      Author: developer
 */

#ifndef RESTCLIENT_H_
#define RESTCLIENT_H_

#include <FApp.h>
#include <FBase.h>
#include "dispatch/dispatch.h"
#include "RestRequestOperation.h"
#include "IRestRequestOwner.h"

class RestClient
: IRestRequestOwner{
public:
	static RestClient& getInstance()
    {
    	static RestClient	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

public:
	void SetBaseUrl(Tizen::Base::String *baseUrl);
	Tizen::Base::String * GetBaseUrl();
	void SetDefaultQueue(dispatch_queue_t queue);
	//dispatch_queue_t GetDefaultQueue();
	dispatch_queue_t __queue;
	void PerformOperation(RestRequestOperation *operation);
	Tizen::Net::Http::HttpSession* GetActiveSession();

private:
	RestClient();
	virtual ~RestClient();
	RestClient(RestClient const&);              // Don't Implement
	void operator = (RestClient const&);
	virtual void OnCompliteN(void *operation);
	Tizen::Net::Http::HttpSession* __pHttpSession;

private:
	Tizen::Base::String *__baseUrl;


};

#endif /* RESTCLIENT_H_ */

/*
 * LongPollConnection.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef LONGPOLLCONNECTION_H_
#define LONGPOLLCONNECTION_H_

#include "RestRequestOperation.h"
#include "IRestRequestListener.h"
#include "RestResponse.h"
#include "Error.h"


using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;

class LongPollConnection
 : IRestRequestListener
 , public ITimerEventListener
{
public:
	static LongPollConnection& getInstance()
    {
    	static LongPollConnection	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

private:
	LongPollConnection();
	virtual ~LongPollConnection();
	LongPollConnection(LongPollConnection const&);              // Don't Implement
	void operator = (LongPollConnection const&);

public:

	//получение информации о LP сервере
	//запрос к LP серверу
	void Run();
	void Stop();
	void GetLongPollServerData();
	void SendRequestToLongPollServer(IList *pArgs);
	void SendRequestToLongPollServer(String *key, String *server, String *ts);

	void Reconnect();

	bool PendingRestart();

	//сохранение TS
	//получение TS

private:
	void OnSuccessN(RestResponse *result);
	void OnErrorN(Error *error);
	virtual void OnTimerExpired (Timer &timer);

	void RunTimer();
	void CancelTimer();

private:
	RestRequestOperation *__pLongPollServerDataOperation;
	RestRequestOperation *__pLongPollConnectionOperation;
	bool __IsRunning;

	String *__pKey;
	String *__pTS;
	String *__pServer;

	bool __pendingRestart;

	Timer *__pPendingTimer;

};

#endif /* LONGPOLLCONNECTION_H_ */

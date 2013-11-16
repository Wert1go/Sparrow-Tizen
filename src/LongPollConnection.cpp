/*
 * LongPollConnection.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "LongPollConnection.h"

#include <FBase.h>
#include <FApp.h>

#include "RestRequestOperation.h"

#include "AuthManager.h"
#include "RestResponse.h"
#include "LongPollResponse.h"
#include "LongPollServerDataResponse.h"
#include "LongPollDescriptor.h"
#include "LongPollServerDataDescriptor.h"
#include "LongPollObject.h"

#include "Error.h"

#include "RestClient.h"

#include "UiMessagesPanel.h"
#include "UiUpdateConstants.h"

#include "MUserDao.h"
#include "MDialogDao.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui::Controls;

LongPollConnection::LongPollConnection() {
	__pLongPollServerDataOperation = null;
	__pLongPollConnectionOperation = null;
	__IsRunning = false;
}

LongPollConnection::~LongPollConnection() {
	__pLongPollServerDataOperation = null;
	__pLongPollConnectionOperation = null;
}

void
LongPollConnection::Run() {
	if (!__IsRunning) {
		__IsRunning = true;
		GetLongPollServerData();
	}
}

void
LongPollConnection::Stop() {
	__IsRunning = false;
}

void
LongPollConnection::GetLongPollServerData() {
	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__pLongPollServerDataOperation) {
		__pLongPollServerDataOperation = new RestRequestOperation(LONGPOLL_GET_SERVER, new String(L"messages.getLongPollServer"), params);
		__pLongPollServerDataOperation->AddEventListener(this);
		__pLongPollServerDataOperation->SetResponseDescriptor(new LongPollServerDataDescriptor());
		RestClient::getInstance().PerformOperation(__pLongPollServerDataOperation);
	}
}

void
LongPollConnection::SendRequestToLongPollServer(IList *pArgs) {

	AppAssert(pArgs->GetCount() == 3);
	String *key = static_cast<String *>(pArgs->GetAt(0));
	String *server = static_cast<String *>(pArgs->GetAt(1));
	String *ts = static_cast<String *>(pArgs->GetAt(2));

	this->SendRequestToLongPollServer(key, server, ts);
}

void
LongPollConnection::SendRequestToLongPollServer(String *key, String *server, String *ts) {
	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());
	params->Add(new String(L"key"), key);
	params->Add(new String(L"act"), new String(L"a_check"));
	params->Add(new String(L"ts"), ts);
	params->Add(new String(L"wait"), new String(L"25"));
	params->Add(new String(L"mode"), new String(L"9"));

	String uri(L"http://");
	uri.Append(server->GetPointer());

	if (!__pLongPollConnectionOperation) {
		__pLongPollConnectionOperation = new RestRequestOperation(new String(uri), LONGPOLL_CONNECTION, null, params);
		__pLongPollConnectionOperation->AddEventListener(this);
		__pLongPollConnectionOperation->SetResponseDescriptor(new LongPollDescriptor());
		RestClient::getInstance().PerformOperation(__pLongPollConnectionOperation);
	}
}

/******************* REST REQUEST LISTENER ******************/

void
LongPollConnection::OnSuccessN(RestResponse *result) {

	if (result->GetOperationCode() == LONGPOLL_GET_SERVER) {
		__pLongPollServerDataOperation->AddEventListener(null);
		__pLongPollServerDataOperation = null;
		LongPollServerDataResponse *longPollServerDataResponse = static_cast<LongPollServerDataResponse *>(result);

		LinkedList *list = new LinkedList();
		list->Add(longPollServerDataResponse->GetKey());
		list->Add(longPollServerDataResponse->GetServer());
		list->Add(longPollServerDataResponse->GetTS());

		App::GetInstance()->SendUserEvent(LONGPOLL_GET_SERVER, list);

	} else if (result->GetOperationCode() == LONGPOLL_CONNECTION) {
		__pLongPollConnectionOperation->AddEventListener(null);
		__pLongPollConnectionOperation = null;
		LongPollResponse *longPollResponse = static_cast<LongPollResponse *>(result);

		//*** test ***//

		//{"ts":1709548768,"updates":[[9,-26033241,1],[8,-183384680,0]]}

		LinkedList *testData = new LinkedList();

		LongPollObject *testObject1 = new LongPollObject();
		testObject1->SetType(LP_USER_ONLINE);
		testObject1->SetUserId(183384680);


		LongPollObject *testObject2 = new LongPollObject();
		testObject2->SetType(LP_USER_OFFLINE);
		testObject2->SetUserId(26033241);

		testData->Add(testObject2);
		testData->Add(testObject1);

		//longPollResponse->SetUpdates(testData);

		//******//


		Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
		LinkedList *pArgs;
		AppAssert(longPollResponse->GetUpdates());

		for (int index = 0; index < longPollResponse->GetUpdates()->GetCount(); index ++) {
			LongPollObject *pObject = static_cast<LongPollObject*>(longPollResponse->GetUpdates()->GetAt(index));

			switch(pObject->GetType()) {
				case LP_DELETE:

					break;
				case LP_FLAG_REPLACE:

						break;
				case LP_FLAG_ADD:

						break;
				case LP_FLAG_RESET:

						break;
				case LP_MESSAGE_ADD:

						break;
				case LP_USER_ONLINE: {
					AppLogDebug("USER %d ONLINE", pObject->GetUserId());

					UiMessagesPanel* pMessagePanel = static_cast< UiMessagesPanel* >(pFrame->GetControl("UiMessagesPanel", true));
					if (pMessagePanel)
					{
						pArgs = new LinkedList();
						pArgs->Add(new Integer(pObject->GetUserId()));
						pMessagePanel->SendUserEvent(UPDATE_USER_ONLINE, pArgs);
						Tizen::App::UiApp::GetInstance()->SendUserEvent(UPDATE_USER_ONLINE, 0);
						pArgs = null;
					}
					MUserDao::getInstance().UpdateUserOnlineStatusById(1, pObject->GetUserId());
					MDialogDao::getInstance().UpdateDialogOnlineStatusById(1, pObject->GetUserId());
				}
						break;
				case LP_USER_OFFLINE: {
					AppLogDebug("USER %d GOES OFFLINE", pObject->GetUserId());

					UiMessagesPanel* pMessagePanel1 = static_cast< UiMessagesPanel* >(pFrame->GetControl("UiMessagesPanel", true));
					if (pMessagePanel1)
					{
						pArgs = new LinkedList();
						pArgs->Add(new Integer(pObject->GetUserId()));
						pMessagePanel1->SendUserEvent(UPDATE_USER_OFFLINE, pArgs);
						Tizen::App::UiApp::GetInstance()->SendUserEvent(UPDATE_USER_OFFLINE, 0);
						pArgs = null;
					}

					MUserDao::getInstance().UpdateUserOnlineStatusById(0, pObject->GetUserId());
					MDialogDao::getInstance().UpdateDialogOnlineStatusById(0, pObject->GetUserId());
				}
						break;
				case LP_CHAT_UPDATED:

						break;
				case LP_USER_PRINT_MSG:

						break;
				case LP_USER_PRINT_MSG_CHAT:

						break;
				}
		}


		if (__IsRunning) {
			App::GetInstance()->SendUserEvent(LONGPOLL_CONNECTION, 0);
		}
	}
}

void
LongPollConnection::OnErrorN(Error *error) {
	if (error->GetCode() == LONG_POLL_REQUEST_RECONNECT) {
		if (__IsRunning) {
			GetLongPollServerData();
		}
	}

	delete error;
}


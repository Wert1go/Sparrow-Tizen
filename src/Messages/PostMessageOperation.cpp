/*
 * PostMessageOperation.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: developer
 */

#include "PostMessageOperation.h"
#include <FBase.h>
#include "Error.h"
#include "MMessage.h"
#include "RestRequestOperation.h"
#include "RestClient.h"
#include "AuthManager.h"
#include "RMessageSendResponse.h"
#include "MMessageDao.h"
#include "RMessageSendDescriptor.h"
#include "IMessageOwner.h"
#include "IMessageDeliveryListener.h"

using namespace Tizen::Base;

PostMessageOperation::PostMessageOperation() {
	__pMessageOwner = null;
	__pDeliveryListener = null;
	__pSendMessageOperation = null;
}

PostMessageOperation::~PostMessageOperation() {
	__pMessageOwner = null;
	__pDeliveryListener = null;
	__pSendMessageOperation = null;
}

void
PostMessageOperation::SetMessageOwner(IMessageOwner *pMessageOwner) {
	__pMessageOwner = pMessageOwner;
}

void
PostMessageOperation::SetDeliveryListener(IMessageDeliveryListener *pDeliveryListener) {
	__pDeliveryListener = pDeliveryListener;
}

void
PostMessageOperation::Perform() {
	AppLogDebug("2");
	SendMessage();
}

void
PostMessageOperation::OnSuccessN(RestResponse *result) {
	if (result->GetOperationCode() == SEND_MESSAGE) {
		RMessageSendResponse *response = static_cast<RMessageSendResponse *>(result);
		if (response->GetError()) {
			AppLogDebug("FAILED TO SEND MESSAGE");
		} else {
			this->__pMessage->SetMid(response->__mid);
			this->__pMessage->SetDelivered(1);
			MMessageDao::getInstance().Save(__pMessage);

			if (__pDeliveryListener) {
				__pDeliveryListener->OnMessageDelivered(__pMessage->GetUid(), __pMessage);
			}
			//save in db
		}

		__pMessageOwner->OnCompliteN(this);
	}
}

void
PostMessageOperation::OnErrorN(Error *error) {
	__pMessageOwner->OnCompliteN(this);
}

void
PostMessageOperation::SendMessage() {
	if (!__pSendMessageOperation) {
		HashMap *params = new HashMap();
		int rawUid = __pMessage->GetUid() < 2000000000 ? __pMessage->GetUid() : __pMessage->GetUid() - 2000000000;
		params->Construct();
		String uid;
		uid.Format(25, L"%d", rawUid);

		if (__pMessage->GetUid() < 2000000000) {

			params->Add(new String(L"user_id"), new String(uid));
		} else {
			params->Add(new String(L"chat_id"), new String(uid));
		}
		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		params->Add(new String(L"message"), __pMessage->GetText());

		__pSendMessageOperation = new RestRequestOperation(SEND_MESSAGE, new String(L"messages.send"), params);
		__pSendMessageOperation->AddEventListener(this);
		__pSendMessageOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__pSendMessageOperation);
	}
}

void
PostMessageOperation::SetMessage(MMessage *message) {
	__pMessage = message;
}

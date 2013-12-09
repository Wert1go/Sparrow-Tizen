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
#include "RMessageSendExecuteDescriptor.h"
#include "IMessageOwner.h"
#include "IMessageDeliveryListener.h"

#include "MAttachment.h"

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

			if (response->__pMessage) {
				this->__pMessage->SetDate(response->__pMessage->GetDate());
			}

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

		String sendMessageRequest(L"var a = API.messages.send({");

		if (__pMessage->GetUid() < 2000000000) {
			sendMessageRequest.Append("\"user_id\" :");
			sendMessageRequest.Append(uid);
		} else {
			sendMessageRequest.Append("\"chat_id\" :");
			sendMessageRequest.Append(uid);
		}

		if (__pMessage->GetText()) {
			sendMessageRequest.Append(",");
			sendMessageRequest.Append("\"message\" :\"");

			__pMessage->GetText()->Replace(L"\n", L"<br>");

			sendMessageRequest.Append(__pMessage->GetText()->GetPointer());
			sendMessageRequest.Append("\"");

			__pMessage->GetText()->Replace(L"<br>", L"\n");
		}

		if (__pMessage->__pAttachments) {
			sendMessageRequest.Append(",");

			String *attachmentString = new String();
			for (int i = 0; i < __pMessage->__pAttachments->GetCount(); i++) {
				MAttachment *attachment = static_cast<MAttachment *>(__pMessage->__pAttachments->GetAt(i));

				attachmentString->Append(attachment->__pType->GetPointer());

				String ownerId;
				String itemId;

				AppLog("%d :: %d", attachment->__ownerId, attachment->__id);

				ownerId.Format(20, L"%d", attachment->__ownerId);
				itemId.Format(20, L"%d", attachment->__id);
				attachmentString->Append(ownerId);
				attachmentString->Append(L"_");
				attachmentString->Append(itemId);

				if (i != __pMessage->__pAttachments->GetCount()-1) {
					attachmentString->Append(L",");
				}
			}

			sendMessageRequest.Append("\"attachment\" : \"");
			sendMessageRequest.Append(attachmentString->GetPointer());
			sendMessageRequest.Append("\"");
			AppLog("ATTACHMENT COUNT: %d :: %S", __pMessage->__pAttachments->GetCount(), attachmentString->GetPointer());
		}


		sendMessageRequest.Append(L"});");
		sendMessageRequest.Append(L"var m = API.messages.getById({\"message_ids\" : a});");
		sendMessageRequest.Append(L"var result = m.items[0];");
		sendMessageRequest.Append(L"return {\"message_id\" : a, \"message\" : result};");

		params->Add(new String(L"code"), new String(sendMessageRequest));
		params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

		AppLog("sendMessageRequest: %S", sendMessageRequest.GetPointer());

		__pSendMessageOperation = new RestRequestOperation(SEND_MESSAGE, new String(L"execute"), params);
		__pSendMessageOperation->AddEventListener(this);
		__pSendMessageOperation->SetResponseDescriptor(new RMessageSendExecuteDescriptor());
		RestClient::getInstance().PerformOperation(__pSendMessageOperation);
	}
}

void
PostMessageOperation::SetMessage(MMessage *message) {
	__pMessage = message;
}

MMessage *
PostMessageOperation::GetMessage() {
	return __pMessage;
}

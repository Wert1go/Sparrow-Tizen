/*
 * PostMan.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#include "PostMan.h"

#include "IMessageDeliveryListener.h"
#include "PostMessageOperation.h"

#include "MAttachment.h"
#include "ImageAttachmentOperation.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

PostMan::PostMan() {
	__mutex.Create();

	__pUidToListenerMap = new HashMapT<Integer*, IMessageDeliveryListener*>();
	__pUidToOperationMap = new HashMapT<Integer*, PostMessageOperation*>();
	__pUidToAttachmentsMap = new HashMapT<Integer*, LinkedList*>();
	__pUidToAttachmentOperationsMap = new HashMapT<Integer*, LinkedList*>();

	__pUidToListenerMap->Construct(100, 0.75);
	__pUidToOperationMap->Construct(100, 0.75);

	__pUidToAttachmentsMap->Construct(100, 0.75);
	__pUidToAttachmentOperationsMap->Construct(100, 0.75);

	__pAttachmentListener = null;

}

PostMan::~PostMan() {
	delete __pUidToListenerMap;
	delete __pUidToOperationMap;

}

void
PostMan::OnCompliteN(PostMessageOperation *operation) {

	AppLog("PostMan::OnCompliteN(PostMessageOperation *operation)");

	__mutex.Acquire();
	Integer *key = this->KeyForUid(operation->__operationId);

	this->__pUidToOperationMap->Remove(key);

	bool exist = false;

	__pUidToListenerMap->ContainsKey(key, exist);

	if (exist) {
		__pUidToListenerMap->Remove(key);
	}

	Integer *attachKey = this->KeyForAttachmentUid(operation->__operationId);

	exist = false;
	this->__pUidToAttachmentsMap->ContainsKey(attachKey, exist);

	if (exist) {
		this->__pUidToAttachmentsMap->Remove(attachKey);
	}

	exist = false;
	this->__pUidToAttachmentOperationsMap->ContainsKey(attachKey, exist);

	if (exist) {
		this->__pUidToAttachmentOperationsMap->Remove(attachKey);
	}

	__mutex.Release();

	delete operation;
	operation = null;

	AppLog("!!PostMan::OnCompliteN(PostMessageOperation *operation)");
}

bool
PostMan::ValidateIncomingMessage(MMessage *pMessage) {

	Integer *key = this->KeyForUid(pMessage->GetUid());

	if (key) {
		return false;
	}

	return true;
}

bool
PostMan::CheckPermissionToSendFromUser(int userId) {
	bool result = false;

	__mutex.Acquire();
	if (this->KeyForUid(userId)) {
		result = true;
	}
	__mutex.Release();

	return result;
}

Integer *
PostMan::KeyForUid(int userId) {
	__mutex.Acquire();
	IListT<Integer *> *keys = __pUidToOperationMap->GetKeysN();

	Integer *pResultKey = null;

	for (int index = 0; index < keys->GetCount(); index++) {
		Integer *key = null;
		keys->GetAt(index, key);

		if (key->ToInt() == userId) {
			pResultKey = key;
			break;
		}
	}

	delete keys;
	__mutex.Release();
	return pResultKey;
}

Integer *
PostMan::KeyForAttachmentUid(int userId) {
	__mutex.Acquire();
	IListT<Integer *> *keys = __pUidToAttachmentsMap->GetKeysN();

	Integer *pResultKey = null;

	for (int index = 0; index < keys->GetCount(); index++) {
		Integer *key = null;
		keys->GetAt(index, key);

		if (key->ToInt() == userId) {
			pResultKey = key;
			break;
		}
	}

	delete keys;
	__mutex.Release();
	return pResultKey;
}

void
PostMan::SendMessageFromUserWithListener(MMessage *pMessage, int userId, IMessageDeliveryListener *pListener) {

	Integer *key = this->KeyForUid(userId);

	if (!key) {

		key = new Integer(userId);
		AppLogDebug("%d", key->ToInt());
		PostMessageOperation *operation = new PostMessageOperation();
		operation->SetMessageOwner(this);
		operation->SetDeliveryListener(pListener);
		operation->__operationId = userId;
		operation->SetMessage(pMessage);


		__mutex.Acquire();

		__pUidToListenerMap->Add(key, pListener);
		__pUidToOperationMap->Add(key, operation);

		__mutex.Release();

		bool readyToRun = true;

		Integer *key = this->KeyForAttachmentUid(userId);
		LinkedList *attachmentList = null;
		LinkedList *attachmentOperationList = null;


		if (key) {
			this->__pUidToAttachmentsMap->GetValue(key, attachmentList);

			this->__pUidToAttachmentOperationsMap->GetValue(key, attachmentOperationList);

			if (attachmentOperationList && attachmentOperationList->GetCount() > 0) {
				readyToRun = false;
			}
		}


		if (readyToRun) {

			if (attachmentList) {
				for (int i = 0; i < attachmentList->GetCount(); i++) {
					MAttachment *attachment = static_cast<MAttachment *>(attachmentList->GetAt(i));

					if (!operation->GetMessage()->__pAttachments) {
						operation->GetMessage()->__pAttachments = new LinkedList();
					}

					operation->GetMessage()->__pAttachments->Add(attachment);
				}
			}

			operation->Perform();
		}

	} else {
		AppLogDebug("STACKOVERFLOW");
	}
}

void
PostMan::RemoveListenerForUser(int userId) {
	__mutex.Acquire();
	Integer *key = this->KeyForUid(userId);
	if (key) {
		bool exist = false;

		__pUidToListenerMap->ContainsKey(key, exist);

		if (exist) {
			__pUidToListenerMap->Remove(key);
		}
	}
	__mutex.Release();
}

/********************** ATTACHMENT *************************/

LinkedList *
PostMan::GetAttachmentsForUid(int uid) {
	Integer *key = this->KeyForAttachmentUid(uid);
	LinkedList *attachmentList = null;
	LinkedList *attachmentOperationList = null;

	this->__pUidToAttachmentsMap->GetValue(key, attachmentList);

	if (attachmentList) {
		return attachmentList;
	} else {
		key = new Integer(uid);

		attachmentList = new LinkedList();
		attachmentOperationList = new LinkedList();

		this->__pUidToAttachmentsMap->Add(key, attachmentList);
		this->__pUidToAttachmentOperationsMap->Add(key, attachmentOperationList);
	}

	return attachmentList;
}

void
PostMan::AddAttachmentWithUid(MAttachment *attachment, int uid) {
	Integer *key = this->KeyForAttachmentUid(uid);
	LinkedList *attachmentList = null;
	LinkedList *attachmentOperationList = null;
	if (!key) {
		key = new Integer(uid);

		attachmentList = new LinkedList();
		attachmentOperationList = new LinkedList();

		this->__pUidToAttachmentsMap->Add(key, attachmentList);
		this->__pUidToAttachmentOperationsMap->Add(key, attachmentOperationList);
	} else {
		this->__pUidToAttachmentsMap->GetValue(key, attachmentList);
		this->__pUidToAttachmentOperationsMap->GetValue(key, attachmentOperationList);
	}

	attachmentList->Add(attachment);

	ImageAttachmentOperation *operation = new ImageAttachmentOperation();
	operation->__pAttachment = attachment;
	operation->__pAttachmentListener = this;
	operation->__uid = uid;

	if (attachmentOperationList->GetCount() == 0) {
		operation->Perform();
	}

	attachmentOperationList->Add(operation);
}

void
PostMan::RemoveAttachmentAtIndexWithUid(int index, int uid) {
	Integer *key = this->KeyForAttachmentUid(uid);
	LinkedList *attachmentList = null;

	AppLog("!DELETE: %d", index);

	if (!key) {
		return;
	}
	AppLog("!!DELETE");
	this->__pUidToAttachmentsMap->GetValue(key, attachmentList);

	if (attachmentList && index < attachmentList->GetCount()) {
		AppLog("DELETE");

		MAttachment *attachment = static_cast<MAttachment *>(attachmentList->GetAt(index));
		attachmentList->RemoveAt(index);
		ImageAttachmentOperation *operation = this->GetOperationForAttachment(key, attachment);

		if (operation) {
			bool isRunning = operation->__IsRunning;
			operation->__pAttachmentListener = null;
			LinkedList *attachmentOperationList = null;
			this->__pUidToAttachmentOperationsMap->GetValue(key, attachmentOperationList);

			attachmentOperationList->Remove(*operation);

			AppLog("TEST");
			if (isRunning) {
				AppLog("TEST11");
				if (attachmentOperationList->GetCount() > 0) {
					AppLog("TEST22");
					operation = static_cast<ImageAttachmentOperation *>(attachmentOperationList->GetAt(0));
					operation->Perform();
				}
			}

		}
	}
}

ImageAttachmentOperation *
PostMan::GetOperationForAttachment(Integer *key, MAttachment *attachment) {
	LinkedList *attachmentOperationList = null;
	this->__pUidToAttachmentOperationsMap->GetValue(key, attachmentOperationList);

	int operationIndex = -1;

	for (int i = 0; i < attachmentOperationList->GetCount(); i++) {
		ImageAttachmentOperation *attachOperation = static_cast<ImageAttachmentOperation *>(attachmentOperationList->GetAt(i));

		if (attachOperation->__pAttachment->__tempId == attachment->__tempId) {
			operationIndex = i;
			break;
		}
	}

	if (operationIndex != -1) {
		ImageAttachmentOperation *operation = static_cast<ImageAttachmentOperation *>(attachmentOperationList->GetAt(operationIndex));
		return operation;
	}

	return null;
}

void
PostMan::OnSuccessN(MAttachment *attachment, int uid) {
	AppLog("OnSuccessN");
	if(__pAttachmentListener) {
		__pAttachmentListener->OnSuccessN(attachment, uid);
	}

	Integer *key = this->KeyForAttachmentUid(uid);

	LinkedList *attachmentList = null;
	LinkedList *attachmentOperationList = null;

	if (!key) {
		return;
	}

	this->__pUidToAttachmentsMap->GetValue(key, attachmentList);
	this->__pUidToAttachmentOperationsMap->GetValue(key, attachmentOperationList);
	ImageAttachmentOperation *operation = this->GetOperationForAttachment(key, attachment);

	if (operation && attachmentOperationList->Contains(*operation)) {
		attachmentOperationList->Remove(*operation);

		if (attachmentOperationList->GetCount() > 0) {
			AppLog("attachmentOperationList->GetCount() > 0");
			operation = static_cast<ImageAttachmentOperation *>(attachmentOperationList->GetAt(0));

			operation->Perform();
		} else {
			AppLog("attachmentOperationList->GetCount() == 0");
			Integer *messageKey = this->KeyForUid(uid);

			PostMessageOperation *operation = null;
			__pUidToOperationMap->GetValue(messageKey, operation);

			if (operation) {
				MMessage *message = operation->GetMessage();

				//add attachments
				for (int i = 0; i < attachmentList->GetCount(); i++) {
					MAttachment *attachment = static_cast<MAttachment *>(attachmentList->GetAt(i));

					if (!message->__pAttachments) {
						message->__pAttachments = new LinkedList();
					}

					message->__pAttachments->Add(attachment);
				}

				if (operation) {
					AppLog("attachmentOperationList->GetCount() == 0 :: SEND");
					operation->Perform();
				}
			}
		}
	}

}

void
PostMan::OnErrorN(Error *error, MAttachment *attachment, int uid) {
	if(__pAttachmentListener) {
		__pAttachmentListener->OnErrorN(error, attachment, uid);
	}
}

void
PostMan::OnProgressChanged(MAttachment *attachment, int progress, int uid) {
	if(__pAttachmentListener) {
		__pAttachmentListener->OnProgressChanged(attachment, progress, uid);
	}
}

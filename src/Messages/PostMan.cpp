/*
 * PostMan.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: developer
 */

#include "PostMan.h"

#include "IMessageDeliveryListener.h"
#include "PostMessageOperation.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;

PostMan::PostMan() {
	__mutex.Create();

	__pUidToListenerMap = new HashMapT<Integer*, IMessageDeliveryListener*>();
	__pUidToOperationMap = new HashMapT<Integer*, PostMessageOperation*>();

	__pUidToListenerMap->Construct(100, 0.75);
	__pUidToOperationMap->Construct(100, 0.75);

}

PostMan::~PostMan() {
	delete __pUidToListenerMap;
	delete __pUidToOperationMap;

}

void
PostMan::OnCompliteN(PostMessageOperation *operation) {
	__mutex.Acquire();
	Integer *key = this->KeyForUid(operation->__operationId);

	this->__pUidToOperationMap->Remove(key);

	bool exist = false;

	__pUidToListenerMap->ContainsKey(key, exist);

	if (exist) {
		__pUidToListenerMap->Remove(key);
	}
	__mutex.Release();

	delete operation;
	operation = null;
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


void
PostMan::SendMessageFromUserWithListener(MMessage *pMessage, int userId, IMessageDeliveryListener *pListener) {

	Integer *key = this->KeyForUid(userId);

	if (!key) {
		AppLogDebug("1");
		key = new Integer(userId);

		PostMessageOperation *operation = new PostMessageOperation();
		operation->SetMessageOwner(this);
		operation->SetDeliveryListener(pListener);
		operation->__operationId = userId;
		operation->SetMessage(pMessage);
		operation->Perform();

		__mutex.Acquire();

		__pUidToListenerMap->Add(key, pListener);
		__pUidToOperationMap->Add(key, operation);

		__mutex.Release();
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

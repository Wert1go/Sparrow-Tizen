/*
 * AuthManager.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#include "AuthManager.h"
#include "RestClient.h"
#include "RestResponse.h"
#include "MUser.h"
#include "ImageCache.h"
#include "UserDescriptor.h"
#include "UserRestResponse.h"
#include "RMessageSendDescriptor.h"

#include <FApp.h>
#include <FGraphics.h>

#include "Helper.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;

#define KEY_ACCESS_TOKEN L"access_token"
#define KEY_USER_ID		 L"user_id"
#define KEY_EXPIRES_IN	 L"expires_id"
#define KEY_FORCED	 	 L"force"

AuthManager::AuthManager() {
	LoadAccessDataFromStore();
	__pUser = null;

	__userOnlineOperation = null;
	__userOfflineOperation = null;
}

AuthManager::~AuthManager() {
	if (__userOnlineOperation) {
		__userOnlineOperation->AddEventListener(null);
	}
	if (__userOfflineOperation) {
		__userOfflineOperation->AddEventListener(null);
	}

	SAFE_DELETE(__userOnlineOperation);
	SAFE_DELETE(__userOfflineOperation);
}

void AuthManager::SaveAccessData(Tizen::Base::String &token, Tizen::Base::String &user_id, Tizen::Base::String &expires_in) {
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();
	result r;

	r = appRegistry->Set(KEY_ACCESS_TOKEN, token);
	r = appRegistry->Set(KEY_USER_ID, user_id);
	r = appRegistry->Set(KEY_EXPIRES_IN, expires_in);

	r = appRegistry->Save();

	LoadAccessDataFromStore();

	if ( __token != null) {
		AppLog("token %S", __token->GetPointer());
	}
}

void AuthManager::LoadAccessDataFromStore() {
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();
	result r;

	String token;
	String userId;
	String expiresIn;

	r = appRegistry->Get(KEY_ACCESS_TOKEN, token);
	r = appRegistry->Get(KEY_USER_ID, userId);
	r = appRegistry->Get(KEY_EXPIRES_IN, expiresIn);

	if (r == E_KEY_NOT_FOUND) {
		appRegistry->Add(KEY_ACCESS_TOKEN, L"");
		appRegistry->Add(KEY_USER_ID, L"");
		appRegistry->Add(KEY_EXPIRES_IN, L"");

		appRegistry->Save();
	}

	if (token != null && userId != null && expiresIn != null) {
		__token = new (std::nothrow) String(token);
		__userId = new (std::nothrow) String(userId);
		__expiresIn = new (std::nothrow) String(expiresIn);
	}

}

bool
AuthManager::IsForced() {
	result r;
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();
	String token;
	r = appRegistry->Get(KEY_FORCED, token);

	if (r == E_KEY_NOT_FOUND) {
		return false;
	}

	return true;
}

void
AuthManager::SetForced(bool state) {
	result r;
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();

	if (state) {
		appRegistry->Add(KEY_FORCED, L"forced");
	} else {
		appRegistry->Remove(KEY_FORCED);
	}

	appRegistry->Save();
}

void
AuthManager::Clear() {
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();
	result r;
	__token = null;
	__userId = null;
	__expiresIn = null;

	appRegistry->Remove(KEY_ACCESS_TOKEN);
	appRegistry->Remove(KEY_USER_ID);
	appRegistry->Remove(KEY_EXPIRES_IN);

	appRegistry->Save();

	LoadAccessDataFromStore();
}

bool AuthManager::IsAuthorized() {
	return __token != null && __userId != null && __expiresIn != null && __token->GetLength() > 0;
}

Tizen::Base::String* AuthManager::AccessToken() {
	return __token;
}

Tizen::Base::String* AuthManager::UserId() {
	return __userId;
}

void
AuthManager::SendRequest() {

	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"user_ids"), AuthManager::getInstance().UserId());
	params->Add(new String(L"fields"), new String(USER_FILEDS));

	if (!__userRequestOperation) {
		__userRequestOperation = new RestRequestOperation(GET_USER, new String(L"users.get"), params);
		__userRequestOperation->AddEventListener(this);
		__userRequestOperation->SetResponseDescriptor(new UserDescriptor());
		RestClient::getInstance().PerformOperation(__userRequestOperation);
	}
}

void
AuthManager::OnSuccessN(RestResponse *response) {

	if (response->GetOperationCode() == GET_USER) {

		if(__userRequestOperation) {
			__userRequestOperation->AddEventListener(null);
			__userRequestOperation = null;
		}

		UserRestResponse *userResponse = static_cast<UserRestResponse *>(response);

		MUser *user = userResponse->GetUser();

		if (user != null && user->GetFirstName() != null) {
			__pUser = user;
		}

		Tizen::App::App::GetInstance()->SendUserEvent(666, 0);
	} else if (response->GetOperationCode() == SET_USER_ONLINE) {
		__userOnlineOperation->AddEventListener(null);
		__userOnlineOperation = null;
	} else if (response->GetOperationCode() == SET_USER_OFFLINE) {
		__userOfflineOperation->AddEventListener(null);
		__userOfflineOperation = null;
	}

}


void
AuthManager::OnErrorN(Error *error) {
	if (__userRequestOperation) {
		__userRequestOperation->AddEventListener(null);
		__userRequestOperation = null;
	}

	delete error;
}

void
AuthManager::OnImageLoadedN(Bitmap *result, Integer *code) {

}

void
AuthManager::UpdateImage() {
	if (this->__pUser) {
		ImageCache::getInstance().LoadImageForTarget(this->__pUser->GetPhoto(), this);
	}
}

void
AuthManager::SetUserOnline() {
	if (!this->IsAuthorized()) {
		return;
	}

	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__userOnlineOperation) {
		__userOnlineOperation = new RestRequestOperation(SET_USER_ONLINE, new String(L"account.setOnline"), params);
		__userOnlineOperation->AddEventListener(this);
		__userOnlineOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__userOnlineOperation);
	}
}

void
AuthManager::SetUserOffline() {
	if (!this->IsAuthorized()) {
		return;
	}

	HashMap *params = new HashMap();
	params->Construct();
	params->Add(new String(L"access_token"), AuthManager::getInstance().AccessToken());

	if (!__userOfflineOperation) {
		__userOfflineOperation = new RestRequestOperation(SET_USER_OFFLINE, new String(L"account.setOffline"), params);
		__userOfflineOperation->AddEventListener(this);
		__userOfflineOperation->SetResponseDescriptor(new RMessageSendDescriptor());
		RestClient::getInstance().PerformOperation(__userOfflineOperation);
	}
}

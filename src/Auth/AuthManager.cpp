/*
 * AuthManager.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#include "AuthManager.h"

using namespace Tizen::App;
using namespace Tizen::Base;

#define KEY_ACCESS_TOKEN L"access_token"
#define KEY_USER_ID		 L"user_id"
#define KEY_EXPIRES_IN	 L"expires_id"

AuthManager::AuthManager() {
	LoadAccessDataFromStore();
}

AuthManager::~AuthManager() {

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

bool AuthManager::IsAuthorized() {
	return __token != null && __userId != null && __expiresIn != null && __token->GetLength() > 0;
}

Tizen::Base::String* AuthManager::AccessToken() {
	return __token;
}

Tizen::Base::String* AuthManager::UserId() {
	return __userId;
}


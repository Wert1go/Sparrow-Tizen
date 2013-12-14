/*
 * AuthManager.h
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#ifndef AUTHMANAGER_H_
#define AUTHMANAGER_H_

#include <FApp.h>
#include <FBase.h>
#include "IRestRequestListener.h"
#include "IImageLoadingListener.h"
#include "RestRequestOperation.h"

class MUser;
using namespace Tizen::Graphics;

class AuthManager
 : public IRestRequestListener
 , public IImageLoadingListener
{
public:
	static AuthManager& getInstance()
    {
    	static AuthManager	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

	void SaveAccessData(Tizen::Base::String &token, Tizen::Base::String &user_id, Tizen::Base::String &expires_in);
	bool IsAuthorized();

	Tizen::Base::String* AccessToken();
	Tizen::Base::String* UserId();
	void Clear();
	bool IsForced();
	void SetForced(bool state);

	void SendRequest(void);
	virtual void OnSuccessN(RestResponse *user);
	virtual void OnErrorN(Error *error);
	virtual void OnImageLoadedN(Bitmap *result, Integer *code);
	void UpdateImage();

	void SetUserOnline();
	void SetUserOffline();

private:
	AuthManager();
	virtual ~AuthManager();
	AuthManager(AuthManager const&);              // Don't Implement
	void operator = (AuthManager const&);

	void LoadAccessDataFromStore();



private:
	Tizen::Base::String* __token;
	Tizen::Base::String* __userId;
	Tizen::Base::String* __expiresIn;
	RestRequestOperation *__userRequestOperation;

	RestRequestOperation *__userOnlineOperation;
	RestRequestOperation *__userOfflineOperation;

	MUser *__pUser;

};

#endif /* AUTHMANAGER_H_ */

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

class AuthManager {
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

};

#endif /* AUTHMANAGER_H_ */

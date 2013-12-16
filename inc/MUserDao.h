/*
 * MUserDao.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef MUSERDAO_H_
#define MUSERDAO_H_

#include "MUser.h"
#include <FIo.h>

using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

class MUserDao {

public:
	static MUserDao& getInstance()
    {
    	static MUserDao	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

private:
	MUserDao();
	virtual ~MUserDao();
	MUserDao(MUserDao const&);              // Don't Implement
	void operator = (MUserDao const&);

public:
	void Save(MUser *user, bool isFriend = true);
	void Save(IList *users, bool isFriends = true);

	MUser *GetCurrentUserN();
	MUser *GetUserN(int uid);
	LinkedList *GetUsersN(String * uids);
	LinkedList *GetUsersByTypeN(int type);

	LinkedList *GetFriendsN(bool onlineOnly = false);
	LinkedList *GetContactsN();
	LinkedList *GetPendingUsersN();

	LinkedList *SearchUsers(String *searchText);

	DbStatement * CreateSaveStatment();
	DbStatement * CreateSaveContactStatment();

	DbStatement * BindUserToSQLStatement(MUser *user, DbStatement *statement);

	MUser * LoadUserFromDBN(DbEnumerator* pEnum);

	void UpdateUserOnlineStatusById(int value, int userId);

};

#endif /* MUSERDAO_H_ */

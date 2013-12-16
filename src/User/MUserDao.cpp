/*
 * MUserDao.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#include "MUserDao.h"
#include "AuthManager.h"
#include "MDatabaseManager.h"

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MUserDao::MUserDao() {


}

MUserDao::~MUserDao() {

}

DbStatement *
MUserDao::CreateSaveStatment() {
	DbStatement *compiledSaveStatment = null;

	String statement;
	statement.Append(L"INSERT OR REPLACE INTO users ("
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo "
			") VALUES ("
			"?, ?, ?, ?, ?, "
			"?, ?, ?, ?, ?, "
			"?, ?, ?, ?"
			")");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	return compiledSaveStatment;
}

void MUserDao::Save(MUser *user, bool isFriend) {

	DbStatement *compiledSaveStatment;
	DbEnumerator* pEnum = null;

		compiledSaveStatment = CreateSaveStatment();
		compiledSaveStatment = BindUserToSQLStatement(user, compiledSaveStatment);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	delete compiledSaveStatment;
	delete pEnum;
}

void MUserDao::Save(IList *users, bool isFriends) {

	DbStatement *compiledSaveStatment;

	compiledSaveStatment = CreateSaveStatment();

	DbEnumerator* pEnum = null;

	IEnumerator* pUserEnum = users->GetEnumeratorN();
	MUser* pUser = null;

	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();
	while (pUserEnum->MoveNext() == E_SUCCESS)
	{
		pUser = dynamic_cast<MUser *>(pUserEnum->GetCurrent());

		compiledSaveStatment = BindUserToSQLStatement(pUser, compiledSaveStatment);

		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
		delete pEnum;
	}
	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pUserEnum;
}

MUser *
MUserDao::GetCurrentUserN() {
	String *uidString = AuthManager::getInstance().UserId();

	int uid;
	Integer::Parse(uidString->GetPointer(), uid);
	MUser *user = this->GetUserN(uid);

	return user;
}

MUser *
MUserDao::GetUserN(int uid) {

	DbEnumerator* pEnum = null;
	String sql;
	MUser *pUser = null;

	sql.Append(L"SELECT "
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo "
			"FROM users "
			"WHERE uid = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, uid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pUser;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pUser = LoadUserFromDBN(pEnum);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUser;
}

LinkedList *
MUserDao::GetUsersByTypeN(int type) {



	return null;
}

LinkedList *
MUserDao::GetFriendsN(bool onlineOnly) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pUsers = new LinkedList();

	MUser *pUser = null;

	sql.Append(L"SELECT "
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo "
			"FROM users "
			"WHERE is_friend = ?");

	if (onlineOnly) {
		sql.Append(L" AND is_online = ?");
	}

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, 1);

	if (onlineOnly) {
		compiledSaveStatment->BindInt(1, 1);
	}

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pUsers;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pUser = LoadUserFromDBN(pEnum);
		pUsers->Add(pUser);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUsers;
}

LinkedList *
MUserDao::GetContactsN() {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pUsers = new LinkedList();

	MUser *pUser = null;

	sql.Append(L"SELECT "
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo "
			"FROM users "
			"WHERE is_contact = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, 1);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pUsers;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pUser = LoadUserFromDBN(pEnum);
		pUsers->Add(pUser);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUsers;
}

LinkedList *
MUserDao::GetUsersN(String * uids) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pUsers = new LinkedList();

	if (!uids) {
		return pUsers;
	}

	MUser *pUser = null;

	sql.Append(L"SELECT "
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo "
			"FROM users "
			"WHERE uid IN ");
	sql.Append(L"(");
	sql.Append(uids->GetPointer());
	sql.Append(L")");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pUsers;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pUser = LoadUserFromDBN(pEnum);
		pUsers->Add(pUser);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUsers;
}

LinkedList *
MUserDao::SearchUsers(String *searchText) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pUsers = new LinkedList();

	MUser *pUser = null;

	sql.Append(L"SELECT "
			"uid, "
			"last_name, "
			"first_name, "
			"photo, "
			"mini_photo, "
			"is_online, "
			"last_seen, "
			"is_friend, "
			"is_contact, "
			"is_pending, "
			"big_photo, "
			"contact_name, "
			"contact_phone, "
			"contact_photo"
			"FROM users "
			"WHERE first_name LIKE '%");
	sql.Append(searchText->GetPointer());
	sql.Append(L"%' OR last_name LIKE '%");
	sql.Append(searchText->GetPointer());
	sql.Append(L"%'");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
	result r = GetLastResult();
	AppLog(GetErrorMessage(r));

	if (!pEnum) {
		return pUsers;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pUser = LoadUserFromDBN(pEnum);
		pUsers->Add(pUser);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUsers;
}

LinkedList *
MUserDao::GetPendingUsersN() {
	return null;
}



DbStatement *
MUserDao::BindUserToSQLStatement(MUser *user, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, user->GetUid());
	statement->BindString(1, user->GetLastName()->GetPointer());
	statement->BindString(2, user->GetFirstName()->GetPointer());
	statement->BindString(3, user->GetPhoto()->GetPointer());
	statement->BindString(4, user->GetMiniPhoto()->GetPointer());
	statement->BindInt(5, user->GetIsOnline());
	statement->BindInt64(6, user->GetLastSeen());
	statement->BindInt(7, user->__isFriend);
	statement->BindInt(8, user->__isContact);
	statement->BindInt(9, user->__isPending);

	if (user->__pBigPhoto) {
		statement->BindString(10, user->__pBigPhoto->GetPointer());
	} else {
		statement->BindString(10, string);
	}

	if (user->__pContactName) {
		statement->BindString(11, user->__pContactName->GetPointer());
	} else {
		statement->BindString(11, string);
	}

	if (user->__pContactPhone) {
		statement->BindString(12, user->__pContactPhone->GetPointer());
	} else {
		statement->BindString(12, string);
	}

	if (user->__pContactPhoto) {
		statement->BindString(13, user->__pContactPhoto->GetPointer());
	} else {
		statement->BindString(13, string);
	}

	return statement;
}

MUser *
MUserDao::LoadUserFromDBN(DbEnumerator* pEnum) {
	MUser *user = new MUser();

	int uid;
	String *firstName = new String();
	String *lastName = new String();
	String *photo = new String();
	String *miniPhoto = new String();
	int isOnline;
	int lastSeen;

	int isFriend;
	int isContact;
	int isPending;

	user->__pBigPhoto = new String(L"");
	user->__pContactName = new String(L"");
	user->__pContactPhone = new String(L"");
	user->__pContactPhoto = new String(L"");

	pEnum->GetIntAt(0, uid);
	pEnum->GetStringAt(1, *lastName);
	pEnum->GetStringAt(2, *firstName);
	pEnum->GetStringAt(3, *photo);
	pEnum->GetStringAt(4, *miniPhoto);
	pEnum->GetIntAt(5, isOnline);
	pEnum->GetIntAt(6, lastSeen);
	pEnum->GetIntAt(7, isFriend);
	pEnum->GetIntAt(8, isContact);
	pEnum->GetIntAt(9, isPending);
	pEnum->GetStringAt(10, *user->__pBigPhoto);
	pEnum->GetStringAt(11, *user->__pContactName);
	pEnum->GetStringAt(12, *user->__pContactPhone);
	pEnum->GetStringAt(13, *user->__pContactPhoto);

	user->SetUid(uid);
	user->SetFirstName(firstName);
	user->SetLastName(lastName);
	user->SetPhoto(photo);
	user->SetMiniPhoto(miniPhoto);
	user->SetIsOnline(isOnline);
	user->SetLastSeen(lastSeen);

	user->__isFriend = isFriend;
	user->__isContact = isContact;
	user->__isPending = isPending;

	return user;
}

void
MUserDao::UpdateUserOnlineStatusById(int value, int userId) {
	String statement;

	statement.Append(L"UPDATE users SET is_online = ? WHERE uid = ?");

	DbEnumerator* pEnum = null;
	DbStatement* pStmt = null;

	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();

	pStmt = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	pStmt->BindInt(0, value);
	pStmt->BindInt(1, userId);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*pStmt);

	AppAssert(!pEnum);

	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pEnum;
	delete pStmt;
}

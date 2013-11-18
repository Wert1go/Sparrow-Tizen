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
	statement.Append(L"INSERT OR REPLACE INTO users (uid, last_name, first_name, photo, mini_photo, is_online, last_seen) VALUES (?, ?, ?, ?, ?, ?, ?)");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	return compiledSaveStatment;
}

void MUserDao::Save(MUser *user) {

	DbStatement *compiledSaveStatment = CreateSaveStatment();
	DbEnumerator* pEnum = null;
	if (compiledSaveStatment) {
		AppLogDebug("Begin Bind2");
	}

	AppLogDebug("Begin Bind");
	compiledSaveStatment = BindUserToSQLStatement(user, compiledSaveStatment);

	AppLogDebug("Perform statement");
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
	AppLogDebug("Complite statement");
	delete compiledSaveStatment;
	delete pEnum;
}

void MUserDao::Save(IList *users) {

	DbStatement *compiledSaveStatment = CreateSaveStatment();
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

	sql.Append(L"SELECT uid, last_name, first_name, photo, mini_photo, is_online, last_seen FROM users WHERE uid = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, uid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pUser;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		AppLogDebug("EXIST pUser!!!");
		pUser = LoadUserFromDBN(pEnum);
	}

	if (pUser) {
		AppLogDebug("EXIST pUser");
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pUser;
}

LinkedList *
MUserDao::GetUsersByTypeN(int type) {



	return null;
}

DbStatement *
MUserDao::BindUserToSQLStatement(MUser *user, DbStatement *statement) {
	if (statement != null) {
		AppLogDebug("Begin Bind1");
	}

	statement->BindInt(0, user->GetUid());
	statement->BindString(1, user->GetLastName()->GetPointer());
	statement->BindString(2, user->GetFirstName()->GetPointer());
	statement->BindString(3, user->GetPhoto()->GetPointer());
	statement->BindString(4, user->GetMiniPhoto()->GetPointer());
	statement->BindInt(5, user->GetIsOnline());
	statement->BindInt64(6, user->GetLastSeen());

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

	pEnum->GetIntAt(0, uid);
	pEnum->GetStringAt(1, *lastName);
	pEnum->GetStringAt(2, *firstName);
	pEnum->GetStringAt(3, *photo);
	pEnum->GetStringAt(4, *miniPhoto);
	pEnum->GetIntAt(5, isOnline);
	pEnum->GetIntAt(6, lastSeen);

	user->SetUid(uid);
	user->SetFirstName(firstName);
	user->SetLastName(lastName);
	user->SetPhoto(photo);
	user->SetMiniPhoto(miniPhoto);
	user->SetIsOnline(isOnline);
	user->SetLastSeen(lastSeen);

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

/*
 * MDialogDao.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#include "MDialogDao.h"
#include "MDialog.h"
#include "Util.h"
#include "MDatabaseManager.h"

#include "MMessage.h"
#include "MUser.h"
#include "MUserDao.h"

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MDialogDao::MDialogDao() {
	// TODO Auto-generated constructor stub

}

MDialogDao::~MDialogDao() {
	// TODO Auto-generated destructor stub
}

void
MDialogDao::Save(MMessage *message, MUser *user) {
	MDialog *tempDialog = new MDialog();
	tempDialog->SetUid(message->GetUid());
	tempDialog->SetIsOnline(user->GetIsOnline());
	if (message->GetText()) {
		tempDialog->SetText(new String(message->GetText()->GetPointer()));
	}
	tempDialog->SetReadState(message->GetReadState());
	if (user->GetFirstName()) {
		tempDialog->SetFirstName(new String(user->GetFirstName()->GetPointer()));
	}
	if (user->GetLastName()) {
		tempDialog->SetLastName(new String(user->GetLastName()->GetPointer()));
	}
	if (user->GetPhoto()) {
		tempDialog->SetPhoto(new String(user->GetPhoto()->GetPointer()));
	}
	if (user->GetMiniPhoto()) {
		tempDialog->SetMiniPhoto(new String(user->GetMiniPhoto()->GetPointer()));
	}
	tempDialog->SetIdentifier(message->GetMid());
	tempDialog->SetOut(message->GetOut());
	tempDialog->SetDate(message->GetDate());

	if (message->uids) {
		tempDialog->SetChatUids(new String(message->uids->GetPointer()));
	}

	if (message->__title) {
		tempDialog->SetTitle(new String(message->__title->GetPointer()));
	}

	if (message->GetChatId() != 0) {
		tempDialog->SetUid(message->GetChatId() + 2000000000);
		tempDialog->SetChatId(message->GetChatId());
	}

	if (message->__pAttachments && message->__pAttachments->GetCount() > 0) {
		tempDialog->__attachmentCount = message->__pAttachments->GetCount();
	}

	if (message->__pGeo) {
		tempDialog->__attachmentCount += 1;
	}

	if (message->__pFwd && message->__pFwd->GetCount() > 0) {
		tempDialog->__fwdCount = message->__pFwd->GetCount();
	}

	this->Save(tempDialog);

	delete tempDialog;
}

void
MDialogDao::Save(MDialog *dialog) {
	DbStatement *compiledSaveStatment = CreateSaveStatement();
	DbEnumerator* pEnum = null;

	compiledSaveStatment = BindDialogToSQLStatement(dialog, compiledSaveStatment);
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	delete compiledSaveStatment;
	delete pEnum;
}

void
MDialogDao::Save(IList *dialogs) {
	DbStatement *compiledSaveStatment = CreateSaveStatement();
	DbEnumerator* pEnum = null;

	IEnumerator* pDialogEnum = dialogs->GetEnumeratorN();
	MDialog* pDialog = null;

	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();

	while (pDialogEnum->MoveNext() == E_SUCCESS)
	{
		pDialog = dynamic_cast<MDialog *>(pDialogEnum->GetCurrent());
		compiledSaveStatment = BindDialogToSQLStatement(pDialog, compiledSaveStatment);
		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
		delete pEnum;
	}

	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pDialogEnum;
}

void
MDialogDao::SaveReaded(int messageId) {
	String statement;

	statement.Append(L"UPDATE dialogs SET read_state = ? WHERE identifier = ?");

	DbEnumerator* pEnum = null;
	DbStatement* pStmt = null;

	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();

	pStmt = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	pStmt->BindInt(0, 1);
	pStmt->BindInt(1, messageId);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*pStmt);

	AppAssert(!pEnum);

	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pEnum;
	delete pStmt;
}

MDialog *
MDialogDao::GetDialogN(int did) {
	DbEnumerator* pEnum = null;
	String sql;
	MDialog *pDialog = null;

	sql.Append(L"SELECT "
			"identifier, uid, last_name, first_name, photo, mini_photo,"
			" is_online, date, out, read_state, title, text, chat_id , chat_uids,"
			" attachment_count,"
			" fwd_count "
			"FROM dialogs "
			"WHERE uid = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, did);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pDialog;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pDialog = LoadDialogFromDBN(pEnum);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pDialog;
}

LinkedList *
MDialogDao::GetDialogsWithOffsetN(int offset) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pDialogs = null;

	sql.Append(L"SELECT "
			"identifier, uid, last_name, first_name, photo, mini_photo, is_online,"
			" date, out, read_state, title, text, chat_id , chat_uids,"
			" attachment_count,"
			" fwd_count "
			"FROM dialogs "
			"ORDER BY date DESC LIMIT 500 OFFSET 0");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pDialogs;
	}

	pDialogs = new LinkedList();
	MDialog *pDialog = null;
	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pDialog = LoadDialogFromDBN(pEnum);
		if (pDialog) {
			pDialogs->Add(pDialog);
		}
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pDialogs;
}

DbStatement *
MDialogDao::CreateSaveStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO dialogs ("
			"identifier, uid, last_name, first_name, photo, mini_photo, is_online,"
			" date, out, read_state, title, text, chat_id, chat_uids,"
			" attachment_count,"
			" fwd_count "
			") VALUES ("
			"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
			")");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	return compiledSaveStatment;
}

DbStatement *
MDialogDao::BindDialogToSQLStatement(MDialog *dialog, DbStatement *statement) {

	String *string = new String (L"");

	statement->BindInt(0, dialog->GetIdentifier());
	statement->BindInt(1, dialog->GetUid());

	if (dialog->GetLastName()) {
		statement->BindString(2, dialog->GetLastName()->GetPointer());
	} else {
		statement->BindString(2, string->GetPointer());
	}

	if (dialog->GetFirstName()) {
		statement->BindString(3, dialog->GetFirstName()->GetPointer());
	} else {
		statement->BindString(3, string->GetPointer());
	}

	if (dialog->GetPhoto()) {
		statement->BindString(4, dialog->GetPhoto()->GetPointer());
	} else {
		statement->BindString(4, string->GetPointer());
	}

	if (dialog->GetMiniPhoto()) {
		statement->BindString(5, dialog->GetMiniPhoto()->GetPointer());
	} else {
		statement->BindString(5, string->GetPointer());
	}

	statement->BindInt(6, dialog->GetIsOnline());
	statement->BindInt64(7, dialog->GetDate());
	statement->BindInt(8, dialog->GetOut());
	statement->BindInt(9, dialog->GetReadState());

	statement->BindString(10, dialog->GetTitle()->GetPointer());

	if (dialog->GetText()) {
		statement->BindString(11, dialog->GetText()->GetPointer());
	} else {
		statement->BindString(11, string->GetPointer());
	}

	if (dialog->GetChatId()) {
		statement->BindInt(12, dialog->GetChatId());
	} else {
		statement->BindInt(12, 0);
	}

	if (dialog->GetChatUids()) {
		statement->BindString(13, dialog->GetChatUids()->GetPointer());
	} else {
		statement->BindString(13, string->GetPointer());
	}

	statement->BindInt(14, dialog->__attachmentCount);
	statement->BindInt(15, dialog->__fwdCount);

	return statement;
}

MDialog *
MDialogDao::LoadDialogFromDBN(DbEnumerator* pEnum) {
	MDialog *dialog = new MDialog();

	int identifier;
	int uid;
	String *firstName = new String();
	String *lastName = new String();
	String *photo = new String();
	String *miniPhoto = new String();
	int isOnline;
	int date;
	int out;
	int readState;
	String *title = new String();
	String *text = new String();
	int chatId;
	String *chatUids = new String();

	pEnum->GetIntAt(0, identifier);
	pEnum->GetIntAt(1, uid);
	pEnum->GetStringAt(2, *lastName);
	pEnum->GetStringAt(3, *firstName);
	pEnum->GetStringAt(4, *photo);
	pEnum->GetStringAt(5, *miniPhoto);
	pEnum->GetIntAt(6, isOnline);
	pEnum->GetIntAt(7, date);
	pEnum->GetIntAt(8, out);
	pEnum->GetIntAt(9, readState);
	pEnum->GetStringAt(10, *title);
	pEnum->GetStringAt(11, *text);
	pEnum->GetIntAt(12, chatId);
	pEnum->GetStringAt(13, *chatUids);

	pEnum->GetIntAt(14, dialog->__attachmentCount);
	pEnum->GetIntAt(15, dialog->__fwdCount);

	dialog->SetIdentifier(identifier);
	dialog->SetUid(uid);
	dialog->SetFirstName(firstName);
	dialog->SetLastName(lastName);
	dialog->SetPhoto(photo);
	dialog->SetMiniPhoto(miniPhoto);
	dialog->SetIsOnline(isOnline);
	dialog->SetDate(date);
	dialog->SetOut(out);
	dialog->SetReadState(readState);
	dialog->SetTitle(title);
	dialog->SetText(text);
	dialog->SetChatId(chatId);
	dialog->SetChatUids(chatUids);

	if (uid > isChatValue) {
		LinkedList *users = MUserDao::getInstance().GetUsersN(chatUids);
		dialog->SetUsers(users);
	}

	return dialog;
}

void
MDialogDao::UpdateDialogOnlineStatusById(int value, int userId) {
	String statement;

	statement.Append(L"UPDATE dialogs SET is_online = ? WHERE uid = ?");

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

void
MDialogDao::DeleteDialog(int uid) {
	String sql(L"DELETE FROM dialogs WHERE uid = ");
	String uidString;
	uidString.Format(11, L"%d", uid);
	sql.Append(uidString);


	AppLog("%S", sql.GetPointer());

	result r;
	r = MDatabaseManager::getInstance().GetDatabase()->ExecuteSql(sql, true);

	AppLog(GetErrorMessage(r));
}

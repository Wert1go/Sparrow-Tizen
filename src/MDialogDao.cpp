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

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MDialogDao::MDialogDao() {
	// TODO Auto-generated constructor stub

}

MDialogDao::~MDialogDao() {
	// TODO Auto-generated destructor stub
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

MDialog *
MDialogDao::GetDialogN(int did) {
	DbEnumerator* pEnum = null;
	String sql;
	MDialog *pDialog = null;

	sql.Append(L"SELECT "
			"identifier, uid, last_name, first_name, photo, mini_photo, is_online, date, out, read_state, title, text "
			"FROM dialogs "
			"WHERE identifier = ?");

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

	if (pDialog) {
		AppLogDebug("EXIST pDialog");
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
			"identifier, uid, last_name, first_name, photo, mini_photo, is_online, date, out, read_state, title, text "
			"FROM dialogs "
			"ORDER BY date DESC LIMIT 20 OFFSET 0");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	//compiledSaveStatment->BindInt(0, offset);

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

	if (pDialogs) {
		AppLogDebug("EXIST pDialogs");
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
			"identifier, uid, last_name, first_name, photo, mini_photo, is_online, date, out, read_state, title, text"
			") VALUES ("
			"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
			")");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	return compiledSaveStatment;
}

DbStatement *
MDialogDao::BindDialogToSQLStatement(MDialog *dialog, DbStatement *statement) {
	if (statement != null) {
		AppLogDebug("BindDialogToSQLStatement::EX");
	}

	statement->BindInt(0, dialog->GetIdentifier());
	statement->BindInt(1, dialog->GetUid());
	statement->BindString(2, dialog->GetLastName()->GetPointer());
	statement->BindString(3, dialog->GetFirstName()->GetPointer());
	statement->BindString(4, dialog->GetPhoto()->GetPointer());
	statement->BindString(5, dialog->GetMiniPhoto()->GetPointer());
	statement->BindInt(6, dialog->GetIsOnline());
	statement->BindInt64(7, dialog->GetDate());
	statement->BindInt(8, dialog->GetOut());
	statement->BindInt(9, dialog->GetReadState());
	statement->BindString(10, dialog->GetTitle()->GetPointer());
	statement->BindString(11, dialog->GetText()->GetPointer());

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

	return dialog;
}

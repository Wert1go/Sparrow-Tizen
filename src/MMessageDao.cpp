/*
 * MMessageDao.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#include "MMessageDao.h"
#include "MMessage.h"
#include "MessageComparer.h"
#include "MDatabaseManager.h"

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MMessageDao::MMessageDao() {
	// TODO Auto-generated constructor stub

}

MMessageDao::~MMessageDao() {
	// TODO Auto-generated destructor stub
}

void
MMessageDao::Save(MMessage *message) {
	DbStatement *compiledSaveStatment = CreateSaveStatement();
	DbEnumerator* pEnum = null;

	compiledSaveStatment = BindMessageToSQLStatement(message, compiledSaveStatment);
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	delete compiledSaveStatment;
	delete pEnum;
}

void
MMessageDao::Save(IList *messages) {
	AppLog("about to save :: %d", messages->GetCount());
	DbStatement *compiledSaveStatment = CreateSaveStatement();
	DbEnumerator* pEnum = null;

	IEnumerator* pMessageEnum = messages->GetEnumeratorN();
	MMessage* pMessage = null;

	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();

	while (pMessageEnum->MoveNext() == E_SUCCESS)
	{

		pMessage = dynamic_cast<MMessage *>(pMessageEnum->GetCurrent());
		compiledSaveStatment = BindMessageToSQLStatement(pMessage, compiledSaveStatment);
		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
		delete pEnum;

	}

	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pMessageEnum;
}

MMessage *
MMessageDao::GetMessageN(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	MMessage *pMessage = null;

	sql.Append(L"SELECT "
			"_id, mid, uid, from_id, date, out, read_state, text "
			"FROM messages "
			"WHERE mid = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, mid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pMessage;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pMessage = LoadMessageFromDBN(pEnum);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pMessage;
}

LinkedList *
MMessageDao::GetMessagesForUser(int userId, int lastMessageId) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pMessages = new LinkedList();

	result r;
	DbStatement *compiledSaveStatment;

	if (lastMessageId != -1) {
		AppLogDebug("test");
		sql.Append(L"SELECT "
					"_id, mid, uid, from_id, date, out, read_state, text "
					"FROM messages "
					"WHERE uid = ? AND mid < ?"
					"ORDER BY date DESC LIMIT 20");
		compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
		r = GetLastResult();
		if (IsFailed(r)) {
		   AppLog(GetErrorMessage(r));
		}

		compiledSaveStatment->BindInt(0, userId);
		compiledSaveStatment->BindInt(1, lastMessageId);

	} else {
		sql.Append(L"SELECT "
							"_id, mid, uid, from_id, date, out, read_state, text "
							"FROM messages "
							"WHERE uid = ?"
							"ORDER BY date DESC LIMIT 20");

		compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
		r = GetLastResult();
		if (IsFailed(r)) {
		   AppLog(GetErrorMessage(r));
		}

		compiledSaveStatment->BindInt(0, userId);
	}

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pMessages;
	}

	pMessages = new LinkedList();
	MMessage *pMessage = null;

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pMessage = LoadMessageFromDBN(pEnum);
		if (pMessage) {
			pMessages->Add(pMessage);
		}
	}


	MessageComparer *comparer = new MessageComparer();
	pMessages->Sort(*comparer);

	delete compiledSaveStatment;
	delete pEnum;

	return pMessages;
}

DbStatement *
MMessageDao::CreateSaveStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO messages (mid, uid, from_id, date, out, read_state, text) VALUES ( ?, ?, ?, ?, ?, ?, ?)");
	result r = E_SUCCESS;
	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	r = GetLastResult();

	if (IsFailed(r))
	{
	   AppLog(GetErrorMessage(r));
	}
	return compiledSaveStatment;
}

DbStatement *
MMessageDao::BindMessageToSQLStatement(MMessage *message, DbStatement *statement) {

	statement->BindInt(0, message->GetMid());
	statement->BindInt(1, message->GetUid());
	statement->BindInt(2, message->GetFromUid());
	statement->BindInt64(3, message->GetDate());
	statement->BindInt(4, message->GetOut());
	statement->BindInt(5, message->GetReadState());
	statement->BindString(6, message->GetText()->GetPointer());

	return statement;
}

MMessage *
MMessageDao::LoadMessageFromDBN(DbEnumerator* pEnum) {
	MMessage *message = new MMessage();
	int _id;
	int mid;
	int uid;
	int fromId;
	int date;
	int out;
	int readState;
	String *text = new String();

	pEnum->GetIntAt(0, _id);
	pEnum->GetIntAt(1, mid);
	pEnum->GetIntAt(2, uid);
	pEnum->GetIntAt(3, fromId);
	pEnum->GetIntAt(4, date);
	pEnum->GetIntAt(5, out);
	pEnum->GetIntAt(6, readState);
	pEnum->GetStringAt(7, *text);

	message->__id = _id;
	message->SetMid(mid);
	message->SetUid(uid);
	message->SetFromUid(fromId);
	message->SetDate(date);
	message->SetOut(out);
	message->SetReadState(readState);
	message->SetText(text);

	return message;
}


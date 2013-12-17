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
#include "MAttachmentDao.h"
#include "MUserDao.h"
#include "MGeo.h"

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MMessageDao::MMessageDao() {
	// TODO Auto-generated constructor stub

}

MMessageDao::~MMessageDao() {
	// TODO Auto-generated destructor stub
}

void
MMessageDao::Save(MMessage *pMessage) {
	DbStatement *compiledSaveStatment = CreateSaveStatement();
	DbEnumerator* pEnum = null;

	compiledSaveStatment = BindMessageToSQLStatement(pMessage, compiledSaveStatment);
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
	if (pMessage->__pAttachments && pMessage->__pAttachments->GetCount() > 0) {
		MAttachmentDao::getInstance().SaveAttachments(pMessage->__pAttachments, pMessage->GetMid());
	}

	if (pMessage->__pGeo) {
		this->SaveGeo(pMessage->__pGeo);
	}

	if (pMessage->__pFwd && pMessage->__pFwd->GetCount() > 0) {
		this->SaveFwd(pMessage->__pFwd, pMessage->GetMid());
	}

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
		if (pMessage->__pAttachments && pMessage->__pAttachments->GetCount() > 0) {
			MAttachmentDao::getInstance().SaveAttachments(pMessage->__pAttachments, pMessage->GetMid());
		}

		if (pMessage->__pGeo) {
			this->SaveGeo(pMessage->__pGeo);
		}

		if (pMessage->__pFwd && pMessage->__pFwd->GetCount() > 0) {
			this->SaveFwd(pMessage->__pFwd, pMessage->GetMid());
		}
	}

	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();

	delete pMessageEnum;

	delete compiledSaveStatment;
}

void
MMessageDao::SaveFwd(IList *pFwdMessages, int mid) {
	DbStatement *compiledSaveFwdStatment = CreateSaveFwdMessageStatement();
	DbStatement *compiledSaveFwsRelationStatment = CreateSaveRelationStatement();
	DbEnumerator* pEnum = null;

	this->DeleteFwdMessageForMessage(mid);

	for (int i = 0; i < pFwdMessages->GetCount(); i++) {
		MMessage *pFwdMessage = static_cast<MMessage *>(pFwdMessages->GetAt(i));

		pFwdMessage->__owner = mid;

		compiledSaveFwdStatment = BindFwdMessageToSQLStatement(pFwdMessage, compiledSaveFwdStatment);
		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveFwdStatment);
		delete pEnum;

		int id = MDatabaseManager::getInstance().GetDatabase()->GetLastInsertRowId();

		id += 1000000000;
		pFwdMessage->SetMid(id);

		if (pFwdMessage->__pAttachments && pFwdMessage->__pAttachments->GetCount() > 0) {
			MAttachmentDao::getInstance().SaveAttachments(pFwdMessage->__pAttachments, pFwdMessage->GetMid(), true);
		}

		if (pFwdMessage->__pGeo) {
			pFwdMessage->__pGeo->__mid = id;
			this->SaveGeo(pFwdMessage->__pGeo);
		}

		if (pFwdMessage->__pFwd && pFwdMessage->__pFwd->GetCount() > 0) {
			this->SaveFwd(pFwdMessage->__pFwd, pFwdMessage->GetMid());
		}
	}

	delete compiledSaveFwdStatment;
	delete compiledSaveFwsRelationStatment;
}

MMessage *
MMessageDao::GetMessageN(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	MMessage *pMessage = null;

	sql.Append(L"SELECT "
			"_id, mid, uid, from_id, date, out, read_state, text, delivered "
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
					"_id, mid, uid, from_id, date, out, read_state, text, delivered "
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
							"_id, mid, uid, from_id, date, out, read_state, text, delivered "
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

	AppLog("LOADED: %d", pMessages->GetCount());

	return pMessages;
}

DbStatement *
MMessageDao::CreateSaveStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO messages (mid, uid, from_id, date, out, read_state, text, delivered) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?)");
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

	String string(L"");

	statement->BindInt(0, message->GetMid());
	statement->BindInt(1, message->GetUid());
	statement->BindInt(2, message->GetFromUid());
	statement->BindInt64(3, message->GetDate());
	statement->BindInt(4, message->GetOut());
	statement->BindInt(5, message->GetReadState());

	if (message->GetText()) {
		statement->BindString(6, message->GetText()->GetPointer());
	} else {
		statement->BindString(6, string);
	}

	statement->BindInt(7, message->GetDelivered());
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
	int delivered;

	pEnum->GetIntAt(0, _id);
	pEnum->GetIntAt(1, mid);
	pEnum->GetIntAt(2, uid);
	pEnum->GetIntAt(3, fromId);
	pEnum->GetIntAt(4, date);
	pEnum->GetIntAt(5, out);
	pEnum->GetIntAt(6, readState);
	pEnum->GetStringAt(7, *text);
	pEnum->GetIntAt(8, delivered);

	message->__id = _id;
	message->SetMid(mid);
	message->SetUid(uid);
	message->SetFromUid(fromId);
	message->SetDate(date);
	message->SetOut(out);
	message->SetReadState(readState);
	message->SetText(text);
	message->SetDelivered(delivered);

	message->__pFwd = this->GetFwdMessages(mid);
	message->__pAttachments = MAttachmentDao::getInstance().GetAttachments(mid);
	message->__pGeo = this->GetGeo(mid);



	return message;
}

LinkedList *
MMessageDao::GetFwdMessages(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pFwdMessage = new LinkedList();

	result r;
	DbStatement *compiledSaveStatment;


	sql.Append(L"SELECT "
			"mid, " //0
			"uid, "			//1
			"date, "		//2
			"text, "	//3
			"owner "	//4

			"FROM fwd_messages WHERE owner = ?");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	r = GetLastResult();
	if (IsFailed(r)) {
	   AppLog(GetErrorMessage(r));
	}

	compiledSaveStatment->BindInt(0, mid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
	if (!pEnum) {
		return pFwdMessage;
	}

	MMessage *pMessage = null;

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pMessage = LoadFwdMessageFromDBN(pEnum);

		if (pMessage) {
			pFwdMessage->Add(pMessage);
		}
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pFwdMessage;
}

void
MMessageDao::DeleteFwdMessageForMessage(int mid) {
	DbEnumerator* pEnum = null;
	String sql;

	result r;
	DbStatement *compiledSaveStatment;


	sql.Append(L"DELETE "

			"FROM fwd_messages WHERE owner = ?");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	r = GetLastResult();
	if (IsFailed(r)) {
	   AppLog(GetErrorMessage(r));
	}

	compiledSaveStatment->BindInt(0, mid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	delete compiledSaveStatment;
	delete pEnum;
}

DbStatement *
MMessageDao::CreateSaveFwdMessageStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO fwd_messages (uid, date, text, owner) VALUES ( ?, ?, ?, ?)");
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
MMessageDao::BindFwdMessageToSQLStatement(MMessage *message, DbStatement *statement) {

//	statement->BindInt(0, message->GetMid());
	statement->BindInt(0, message->GetUid());
	statement->BindInt64(1, message->GetDate());
	statement->BindString(2, message->GetText()->GetPointer());
	statement->BindInt(3, message->__owner);

	return statement;
}

MMessage *
MMessageDao::LoadFwdMessageFromDBN(DbEnumerator* pEnum) {
	MMessage *message = new MMessage();
	int mid;
	int uid;
	int date;
	String *text = new String();
	int owner;

	pEnum->GetIntAt(0, mid);
	pEnum->GetIntAt(1, uid);
	pEnum->GetIntAt(2, date);
	pEnum->GetStringAt(3, *text);
	pEnum->GetIntAt(4, owner);

	message->SetMid(mid + 1000000000);
	message->SetUid(uid);
	message->SetDate(date);
	message->SetText(text);
	message->__owner = owner;

	message->__pType = new String(L"fwd");
	message->__pUser = MUserDao::getInstance().GetUserN(message->GetUid());
	message->__pFwd = this->GetFwdMessages(message->GetMid());
	message->__pAttachments = MAttachmentDao::getInstance().GetAttachments(message->GetMid(), true);
	message->__pGeo = this->GetGeo(message->GetMid());

	return message;
}

DbStatement *
MMessageDao::CreateSaveRelationStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO m_to_fm_relations (fmid, aid) VALUES (?, ?)");
	result r = E_SUCCESS;
	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	r = GetLastResult();

	if (IsFailed(r))
	{
	   AppLog(GetErrorMessage(r));
	}
	return compiledSaveStatment;
}

/*************************** UTILS **********************/

void
MMessageDao::SaveReaded(int messageId) {
	String statement;

	statement.Append(L"UPDATE messages SET read_state = ? WHERE mid = ?");

	DbEnumerator* pEnum = null;
	DbStatement* pStmt = null;

	pStmt = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	pStmt->BindInt(0, 1);
	pStmt->BindInt(1, messageId);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*pStmt);

	AppAssert(!pEnum);

	delete pEnum;
	delete pStmt;
}

void
MMessageDao::markAsReaded(int userId, int chatId) {
	String statement;

	statement.Append(L"UPDATE messages SET read_state = ? WHERE uid = ? AND out = ?");

	DbEnumerator* pEnum = null;
	DbStatement* pStmt = null;

	pStmt = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	pStmt->BindInt(0, 1);
	pStmt->BindInt(1, userId);
	pStmt->BindInt(2, 0);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*pStmt);

	AppAssert(!pEnum);

	delete pEnum;
	delete pStmt;
}

int
MMessageDao::firstUnreadMessage(int userId, int chatId) {
	DbEnumerator* pEnum = null;
	String sql;
	int id = -1;

	result r;
	DbStatement *compiledSaveStatment;

	sql.Append(L"SELECT mid FROM messages "
				"WHERE uid = ? AND read_state = ? AND out = ? "
				"ORDER BY mid ASC LIMIT 1");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	r = GetLastResult();
	if (IsFailed(r)) {
	   AppLog(GetErrorMessage(r));
	}

	compiledSaveStatment->BindInt(0, userId);
	compiledSaveStatment->BindInt(1, 0);
	compiledSaveStatment->BindInt(2, 0);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return id;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pEnum->GetIntAt(0, id);
	}

	delete pEnum;

	return id;
}

int
MMessageDao::GetUnreadCount() {
	DbEnumerator* pEnum = null;
	String sql;
	int count = 0;

	result r;
	DbStatement *compiledSaveStatment;

	sql.Append(L"SELECT mid FROM messages "
				"WHERE read_state = ? AND out = ? ");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	r = GetLastResult();
	if (IsFailed(r)) {
	   AppLog(GetErrorMessage(r));
	}

	compiledSaveStatment->BindInt(0, 0);
	compiledSaveStatment->BindInt(1, 0);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return count;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		count++;
	}

	delete pEnum;

	return count;
}

/*************************** GEO ***************************/

DbStatement *
MMessageDao::CreateSaveGeoStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO geo ("
				"mid, "
				"type, "
				"coordinates, "
				"place_title, "
				"place_country, "
				"place_city "
			") VALUES ("
				" ?, ?, ?, ?, ?, ?"
			")");

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
MMessageDao::BindGeoToSQLStatement(MGeo *geo, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, geo->__mid);

	if (geo->__pType) {
		statement->BindString(1, geo->__pType->GetPointer());
	} else {
		statement->BindString(1, string);
	}

	if (geo->__pCoordinates) {
		statement->BindString(2, geo->__pCoordinates->GetPointer());
	} else {
		statement->BindString(2, string);
	}

	if (geo->__pPlaceTitle) {
		statement->BindString(3, geo->__pPlaceTitle->GetPointer());
	} else {
		statement->BindString(3, string);
	}

	if (geo->__pPlaceCountry) {
		statement->BindString(4, geo->__pPlaceCountry->GetPointer());
	} else {
		statement->BindString(4, string);
	}

	if (geo->__pPlaceCity) {
		statement->BindString(5, geo->__pPlaceCity->GetPointer());
	} else {
		statement->BindString(5, string);
	}

	return statement;
}

MGeo *
MMessageDao::GetGeo(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	MGeo *pGeo = null;

	sql.Append(L"SELECT "
				"mid, "
				"type, "
				"coordinates, "
				"place_title, "
				"place_country, "
				"place_city "
			"FROM geo "
			"WHERE mid = ?");

	DbStatement *compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);
	compiledSaveStatment->BindInt(0, mid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pGeo;
	}

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pGeo = LoadGeoFromDBN(pEnum);
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pGeo;
}

MGeo *
MMessageDao::LoadGeoFromDBN(DbEnumerator* pEnum) {
	MGeo *geo = new MGeo();

	String *pType = new String(L"");
	String *pCoordinates = new String(L"");

	String *pPlaceTitle = new String(L"");
	String *pPlaceCity = new String(L"");
	String *pPlaceCountry = new String(L"");

	pEnum->GetIntAt(0, geo->__mid);

	pEnum->GetStringAt(1, *pType);
	pEnum->GetStringAt(2, *pCoordinates);

	pEnum->GetStringAt(3, *pPlaceTitle);
	pEnum->GetStringAt(4, *pPlaceCountry);
	pEnum->GetStringAt(5, *pPlaceCity);

	geo->__pType = pType;
	geo->__pCoordinates = pCoordinates;

	geo->SetPlaceTitle(pPlaceTitle);
	geo->__pPlaceCity = pPlaceCity;
	geo->__pPlaceCountry = pPlaceCountry;

	return geo;
}

void
MMessageDao::SaveGeo(MGeo *geo) {
	DbStatement *compiledSaveStatment = CreateSaveGeoStatement();
	DbEnumerator* pEnum = null;

	compiledSaveStatment = BindGeoToSQLStatement(geo, compiledSaveStatment);
	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	delete compiledSaveStatment;
	delete pEnum;
}


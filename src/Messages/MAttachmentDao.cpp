/*
 * MAttachmentDao.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#include <FBase.h>
#include "MAttachmentDao.h"
#include "MAttachment.h"
#include "MDatabaseManager.h"

using namespace Tizen::Io;
using namespace Tizen::Base::Collection;

MAttachmentDao::MAttachmentDao() {
	// TODO Auto-generated constructor stub

}

MAttachmentDao::~MAttachmentDao() {
	// TODO Auto-generated destructor stub
}

DbStatement *
MAttachmentDao::CreateSavePhotoStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO attachments ("
							"identifier, "
							"mid, "
							"type, "
							"owner_id, "
							"date, "
							"access_key, "
							"width, "
							"height, "
							"photo_130, "
							"photo_604 "
			") VALUES ("
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?, ?)");

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
MAttachmentDao::BindPhotoToSQLStatement(MAttachment *photo, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, photo->__id);
	statement->BindInt(1, photo->__mid);
	statement->BindString(2, photo->__pType->GetPointer());
	statement->BindInt(3, photo->__ownerId);
	statement->BindInt(4, photo->__date);
	statement->BindString(5, photo->__pAccessKey->GetPointer());
	statement->BindInt(6, photo->__width);
	statement->BindInt(7, photo->__height);
	if (photo->__pPhoto130) {
		statement->BindString(8, photo->__pPhoto130->GetPointer());
	} else {
		statement->BindString(8, string);
	}
	if (photo->__pPhoto604) {
		statement->BindString(9, photo->__pPhoto604->GetPointer());
	} else {
		statement->BindString(9, string);
	}
	return statement;
}

MAttachment *
MAttachmentDao::LoadPhotoFromDBN(DbEnumerator* pEnum) {
	MAttachment *attachment = new MAttachment();

	attachment->__pType = new String(L"");
	attachment->__pAccessKey = new String(L"");
	attachment->__pPhoto130 = new String (L"");
	attachment->__pPhoto604 = new String(L"");

	pEnum->GetIntAt(0, attachment->__id);
	pEnum->GetIntAt(1, attachment->__mid);
	pEnum->GetStringAt(2, *attachment->__pType);
	pEnum->GetIntAt(3, attachment->__ownerId);
	pEnum->GetIntAt(4, attachment->__date);
	pEnum->GetStringAt(5, *attachment->__pAccessKey);
	pEnum->GetIntAt(6, attachment->__width);
	pEnum->GetIntAt(7, attachment->__height);

	pEnum->GetStringAt(8, *attachment->__pPhoto130);
	pEnum->GetStringAt(9, *attachment->__pPhoto604);

	return attachment;
}

void
MAttachmentDao::SaveAttachments(IList *pAttachments, int mid) {
//	AppLog("SaveAttachments %d", mid);
	DbStatement *compiledSaveStatment = CreateSavePhotoStatement();
	DbEnumerator* pEnum = null;

	IEnumerator* pAttachEnum = pAttachments->GetEnumeratorN();
	MAttachment* pAttachment = null;

//	MDatabaseManager::getInstance().GetDatabase()->BeginTransaction();

	while (pAttachEnum->MoveNext() == E_SUCCESS)
	{

		pAttachment = dynamic_cast<MAttachment *>(pAttachEnum->GetCurrent());
		pAttachment->__mid = mid;
		compiledSaveStatment = BindPhotoToSQLStatement(pAttachment, compiledSaveStatment);
		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
		delete pEnum;
	}

//	MDatabaseManager::getInstance().GetDatabase()->CommitTransaction();
	delete pAttachEnum;
}

void
MAttachmentDao::SavePhoto(MAttachment *photo, int mid) {

}

LinkedList *
MAttachmentDao::GetAttachments(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pAttachments = new LinkedList();

	result r;
	DbStatement *compiledSaveStatment;


	sql.Append(L"SELECT "
			"identifier, "
			"mid, "
			"type, "
			"owner_id, "
			"date, "
			"access_key, "
			"width, "
			"height, "
			"photo_130, "
			"photo_604 "
			"FROM attachments "
			"WHERE mid = ?");

	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(sql);

	r = GetLastResult();
	if (IsFailed(r)) {
	   AppLog(GetErrorMessage(r));
	}

	compiledSaveStatment->BindInt(0, mid);

	pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);

	if (!pEnum) {
		return pAttachments;
	}

	MAttachment *pAttachment = null;

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pAttachment = LoadPhotoFromDBN(pEnum);
		if (pAttachment) {
			pAttachments->Add(pAttachment);
		}
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pAttachments;
}

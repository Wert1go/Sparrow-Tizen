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

/************************* PHOTO ****************************/

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
							"photo_604, "
							"album_id "
			") VALUES ("
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?, ?, ?)");

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
	if (photo->__pAccessKey) {
		statement->BindString(5, photo->__pAccessKey->GetPointer());
	} else {
		statement->BindString(5, string);
	}
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
	statement->BindInt(10, photo->__album_id);

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
	pEnum->GetIntAt(10, attachment->__album_id);

	return attachment;
}

/************************* VIDEO ****************************/

DbStatement *
MAttachmentDao::CreateSaveVideoStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO attachments ("
							"identifier, "
							"mid, "
							"type, "
							"owner_id, "
							"date, "

							"access_key, "
							"photo_130, "
							"photo_320, "
							"album_id, "

							"title, "
							"duration, "
							"views, "
							"description "

			") VALUES ("
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?)");

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
MAttachmentDao::BindVideoToSQLStatement(MAttachment *pAttach, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, pAttach->__id);
	statement->BindInt(1, pAttach->__mid);
	statement->BindString(2, pAttach->__pType->GetPointer());
	statement->BindInt(3, pAttach->__ownerId);
	statement->BindInt(4, pAttach->__date);

	if (pAttach->__pAccessKey) {
		statement->BindString(5, pAttach->__pAccessKey->GetPointer());
	} else {
		statement->BindString(5, string);
	}
	if (pAttach->__pPhoto130) {
		statement->BindString(6, pAttach->__pPhoto130->GetPointer());
	} else {
		statement->BindString(6, string);
	}

	if (pAttach->__pVideoPhoto320) {
		statement->BindString(7, pAttach->__pVideoPhoto320->GetPointer());
	} else {
		statement->BindString(7, string);
	}

	statement->BindInt(8, pAttach->__album_id);

	if (pAttach->__pTitle) {
		statement->BindString(9, pAttach->__pTitle->GetPointer());
	} else {
		statement->BindString(9, string);
	}

	statement->BindInt(10, pAttach->__duration);
	statement->BindInt(11, pAttach->__views);

	if (pAttach->__pDescription) {
		statement->BindString(12, pAttach->__pDescription->GetPointer());
	} else {
		statement->BindString(12, string);
	}

	return statement;
}

MAttachment *
MAttachmentDao::LoadVideoFromDBN(DbEnumerator* pEnum) {
	MAttachment *attachment = new MAttachment();

	attachment->__pType = new String(L"");
	attachment->__pAccessKey = new String(L"");
	attachment->__pPhoto130 = new String (L"");
	attachment->__pVideoPhoto320 = new String(L"");
	attachment->__pTitle = new String(L"");
	attachment->__pDescription = new String(L"");

	pEnum->GetIntAt(0, attachment->__id);
	pEnum->GetIntAt(1, attachment->__mid);
	pEnum->GetStringAt(2, *attachment->__pType);
	pEnum->GetIntAt(3, attachment->__ownerId);
	pEnum->GetIntAt(4, attachment->__date);
	pEnum->GetStringAt(5, *attachment->__pAccessKey);

	pEnum->GetStringAt(6, *attachment->__pPhoto130);
	pEnum->GetStringAt(11, *attachment->__pVideoPhoto320);

	pEnum->GetIntAt(10, attachment->__album_id);

	pEnum->GetStringAt(12, *attachment->__pTitle);
	pEnum->GetIntAt(13, attachment->__duration);
	pEnum->GetIntAt(14, attachment->__views);
	pEnum->GetStringAt(15, *attachment->__pDescription);

	return attachment;
}

/************************ COMMON METHODS **************************/

void
MAttachmentDao::SaveAttachments(IList *pAttachments, int mid) {
	AppLog("SaveAttachments %d :: count %d", mid, pAttachments->GetCount());

	DbStatement *compiledSavePhotoStatment = CreateSavePhotoStatement();
	DbStatement *compiledSaveVideoStatment = CreateSaveVideoStatement();
	DbEnumerator* pEnum = null;

	IEnumerator* pAttachEnum = pAttachments->GetEnumeratorN();
	MAttachment* pAttachment = null;

	while (pAttachEnum->MoveNext() == E_SUCCESS)
	{
		DbStatement *compiledSaveStatment = null;

		pAttachment = dynamic_cast<MAttachment *>(pAttachEnum->GetCurrent());
		pAttachment->__mid = mid;

		if (pAttachment->__pType->Equals(PHOTO, false)) {
			compiledSaveStatment = BindPhotoToSQLStatement(pAttachment, compiledSavePhotoStatment);
		} else if (pAttachment->__pType->Equals(VIDEO, false)) {
			compiledSaveStatment = BindVideoToSQLStatement(pAttachment, compiledSaveVideoStatment);
		}

		if (compiledSaveStatment) {
			pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
			delete pEnum;
		}

	}

	delete pAttachEnum;
	delete compiledSavePhotoStatment;
	delete compiledSaveVideoStatment;
}

LinkedList *
MAttachmentDao::GetAttachments(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pAttachments = new LinkedList();

	result r;
	DbStatement *compiledSaveStatment;


	sql.Append(L"SELECT "
			"identifier, " //0
			"mid, "			//1
			"type, "		//2
			"owner_id, "	//3
			"date, "		//4
			"access_key, "	//4
			"width, "		//6
			"height, "		//7
			"photo_130, "	//8
			"photo_604, "	//9

			"album_id,"		//10

			"photo_320, "	//11
			"title, "		//12
			"duration, "	//13
			"views, "		//14
			"description, "	//15

			"artist, "		//16
			"url, "			//17
			"size, "		//18
			"ext "			//19

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
		String *pType = new String(L"");
		pEnum->GetStringAt(2, *pType);

		if (pType->Equals(PHOTO, false)) {
			pAttachment = LoadPhotoFromDBN(pEnum);
		} else if (pType->Equals(VIDEO, false)) {
			pAttachment = LoadVideoFromDBN(pEnum);
		}

		if (pAttachment) {
			pAttachments->Add(pAttachment);
		}

		delete pType;
	}

	delete compiledSaveStatment;
	delete pEnum;

	return pAttachments;
}

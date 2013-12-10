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

/************************* AUDIO ****************************/

DbStatement *
MAttachmentDao::CreateSaveAudioStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO attachments ("
							"identifier, "
							"mid, "
							"type, "

							"owner_id, "

							"artist, "
							"title, "
							"duration, "
							"url "

			") VALUES ("
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?"
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
MAttachmentDao::BindAudioToSQLStatement(MAttachment *pAttach, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, pAttach->__id);
	statement->BindInt(1, pAttach->__mid);
	statement->BindString(2, pAttach->__pType->GetPointer());
	statement->BindInt(3, pAttach->__ownerId);


	if (pAttach->__pArtist) {
		statement->BindString(4, pAttach->__pArtist->GetPointer());
	} else {
		statement->BindString(4, string);
	}

	if (pAttach->__pTitle) {
		statement->BindString(5, pAttach->__pTitle->GetPointer());
	} else {
		statement->BindString(5, string);
	}

	statement->BindInt(6, pAttach->__duration);


	if (pAttach->__pUrl) {
		statement->BindString(7, pAttach->__pUrl->GetPointer());
	} else {
		statement->BindString(7, string);
	}

	return statement;
}

MAttachment *
MAttachmentDao::LoadAudioFromDBN(DbEnumerator* pEnum) {
	MAttachment *attachment = new MAttachment();

	attachment->__pType = new String(L"");
	attachment->__pTitle = new String(L"");
	attachment->__pUrl = new String(L"");
	attachment->__pArtist = new String(L"");

	pEnum->GetIntAt(0, attachment->__id);
	pEnum->GetIntAt(1, attachment->__mid);
	pEnum->GetStringAt(2, *attachment->__pType);
	pEnum->GetIntAt(3, attachment->__ownerId);

	pEnum->GetStringAt(12, *attachment->__pTitle);
	pEnum->GetIntAt(13, attachment->__duration);

	pEnum->GetStringAt(16, *attachment->__pArtist);
	pEnum->GetStringAt(17, *attachment->__pUrl);

	return attachment;
}

/************************* DOC ****************************/

DbStatement *
MAttachmentDao::CreateSaveDocStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO attachments ("
							"identifier, "
							"mid, "
							"type, "
							"access_key, "

							"owner_id, "

							"ext, "
							"title, "
							"size, "
							"url "

			") VALUES ("
			" ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?"
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
MAttachmentDao::BindDocToSQLStatement(MAttachment *pAttach, DbStatement *statement) {

	String string(L"");

	statement->BindInt(0, pAttach->__id);
	statement->BindInt(1, pAttach->__mid);
	statement->BindString(2, pAttach->__pType->GetPointer());


	if (pAttach->__pAccessKey) {
		statement->BindString(3, pAttach->__pAccessKey->GetPointer());
	} else {
		statement->BindString(3, string);
	}

	statement->BindInt(4, pAttach->__ownerId);

	if (pAttach->__pExt) {
		statement->BindString(5, pAttach->__pExt->GetPointer());
	} else {
		statement->BindString(5, string);
	}

	if (pAttach->__pTitle) {
		statement->BindString(6, pAttach->__pTitle->GetPointer());
	} else {
		statement->BindString(6, string);
	}

	statement->BindInt(7, pAttach->__size);


	if (pAttach->__pUrl) {
		statement->BindString(8, pAttach->__pUrl->GetPointer());
	} else {
		statement->BindString(8, string);
	}

	return statement;
}

MAttachment *
MAttachmentDao::LoadDocFromDBN(DbEnumerator* pEnum) {
	MAttachment *attachment = new MAttachment();

	attachment->__pType = new String(L"");
	attachment->__pTitle = new String(L"");
	attachment->__pUrl = new String(L"");
	attachment->__pExt = new String(L"");
	attachment->__pAccessKey = new String(L"");

	pEnum->GetIntAt(0, attachment->__id);
	pEnum->GetIntAt(1, attachment->__mid);
	pEnum->GetStringAt(2, *attachment->__pType);
	pEnum->GetIntAt(3, attachment->__ownerId);

	pEnum->GetStringAt(5, *attachment->__pAccessKey);

	pEnum->GetStringAt(12, *attachment->__pTitle);
	pEnum->GetIntAt(18, attachment->__size);

	pEnum->GetStringAt(19, *attachment->__pExt);
	pEnum->GetStringAt(17, *attachment->__pUrl);

	return attachment;
}

/************************ COMMON METHODS **************************/

DbStatement *
MAttachmentDao::CreateSaveRelationStatement() {
	DbStatement *compiledSaveStatment = null;

	String statement;

	statement.Append(L"INSERT OR REPLACE INTO m_to_a_relations (mid, aid) VALUES (?, ?)");
	result r = E_SUCCESS;
	compiledSaveStatment = MDatabaseManager::getInstance().GetDatabase()->CreateStatementN(statement);

	r = GetLastResult();

	if (IsFailed(r))
	{
	   AppLog(GetErrorMessage(r));
	}
	return compiledSaveStatment;
}

void
MAttachmentDao::SaveAttachments(IList *pAttachments, int mid) {
	AppLog("SaveAttachments %d :: count %d", mid, pAttachments->GetCount());

	DbStatement *compiledSaveRelationStatment = CreateSaveRelationStatement();

	DbStatement *compiledSavePhotoStatment = CreateSavePhotoStatement();
	DbStatement *compiledSaveVideoStatment = CreateSaveVideoStatement();
	DbStatement *compiledSaveAudioStatment = CreateSaveAudioStatement();
	DbStatement *compiledSaveDocStatment = CreateSaveDocStatement();

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
		} else if (pAttachment->__pType->Equals(AUDIO, false)) {
			compiledSaveStatment = BindAudioToSQLStatement(pAttachment, compiledSaveAudioStatment);
		} else if (pAttachment->__pType->Equals(DOC, false)) {
			compiledSaveStatment = BindDocToSQLStatement(pAttachment, compiledSaveDocStatment);
		}

		if (compiledSaveStatment) {
			pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveStatment);
			delete pEnum;
		}

		compiledSaveRelationStatment->BindInt(0, mid);
		compiledSaveRelationStatment->BindInt(1, pAttachment->__id);

		pEnum = MDatabaseManager::getInstance().GetDatabase()->ExecuteStatementN(*compiledSaveRelationStatment);
		delete pEnum;

	}

	delete pAttachEnum;

	delete compiledSavePhotoStatment;
	delete compiledSaveVideoStatment;
	delete compiledSaveAudioStatment;
	delete compiledSaveDocStatment;
}

LinkedList *
MAttachmentDao::GetAttachments(int mid) {
	DbEnumerator* pEnum = null;
	String sql;
	LinkedList *pAttachments = new LinkedList();

	result r;
	DbStatement *compiledSaveStatment;


	sql.Append(L"SELECT "
			"a.identifier, " //0
			"a.mid, "			//1
			"a.type, "		//2
			"a.owner_id, "	//3
			"a.date, "		//4
			"a.access_key, "	//5
			"a.width, "		//6
			"a.height, "		//7
			"a.photo_130, "	//8
			"a.photo_604, "	//9

			"a.album_id,"		//10

			"a.photo_320, "	//11
			"a.title, "		//12
			"a.duration, "	//13
			"a.views, "		//14
			"a.description, "	//15

			"a.artist, "		//16
			"a.url, "			//17
			"a.size, "		//18
			"a.ext "			//19

			"FROM attachments as a "
			"INNER JOIN m_to_a_relations as d "
			"ON d.mid = ? AND a.identifier = d.aid"
			);

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
		} else if (pType->Equals(AUDIO, false)) {
			pAttachment = LoadAudioFromDBN(pEnum);
		} else if (pType->Equals(DOC, false)) {
			pAttachment = LoadDocFromDBN(pEnum);
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

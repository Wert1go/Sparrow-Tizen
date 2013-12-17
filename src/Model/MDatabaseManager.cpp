/*
 * DataBaseManager.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: developer
 */

#include "MDatabaseManager.h"

#include <FIo.h>
#include <FApp.h>

#include "MUser.h"
#include "MDialog.h"
#include "MMessage.h"
#include "MAttachment.h"
#include "MGeo.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Io;

MDatabaseManager::MDatabaseManager() {
	String dbPath(Tizen::App::App::GetInstance()->GetAppDataPath() + L"vk.db");
	String sql;
	result r = E_SUCCESS;
	__database = new Database();
	r = __database->Construct(dbPath, "a+");

	String *sqlUser = MUser::TableDescription();
	r = __database->ExecuteSql(sqlUser->GetPointer(), true);
	delete sqlUser;

	String *sqlContact = MUser::TableContactsDescription();
	r = __database->ExecuteSql(sqlContact->GetPointer(), true);
	delete sqlContact;

	String *sqlDialog = MDialog::TableDescription();
	r = __database->ExecuteSql(sqlDialog->GetPointer(), true);
	delete sqlDialog;

	String *sqlMessage = MMessage::TableDescription();
	r = __database->ExecuteSql(sqlMessage->GetPointer(), true);
	delete sqlMessage;

	String *sqlMtoARelation = MMessage::RelationTableDescription();
	r = __database->ExecuteSql(sqlMtoARelation->GetPointer(), true);
	AppLog(GetErrorMessage(r));
	delete sqlMtoARelation;

	String *sqlFwdMessage = MMessage::FwdTableDescription();
	r = __database->ExecuteSql(sqlFwdMessage->GetPointer(), true);
	AppLog(GetErrorMessage(r));
	delete sqlFwdMessage;

	String *sqlMtoFMRelation = MMessage::FwdRelationTableDescription();
	r = __database->ExecuteSql(sqlMtoFMRelation->GetPointer(), true);
	AppLog(GetErrorMessage(r));
	delete sqlMtoFMRelation;

	String *sqlGeo = MGeo::TableDescription();
	r = __database->ExecuteSql(sqlGeo->GetPointer(), true);
	delete sqlGeo;

	String *sqlAttachment = MAttachment::TableDescription();
	r = __database->ExecuteSql(sqlAttachment->GetPointer(), true);
	delete sqlAttachment;
}

void
MDatabaseManager::Clear() {
	result r = E_SUCCESS;

	String *sqlUser = new String(L"DELETE FROM users");
	r = __database->ExecuteSql(sqlUser->GetPointer(), true);
	delete sqlUser;

	String *sqlDialog = new String(L"DELETE FROM dialogs");
	r = __database->ExecuteSql(sqlDialog->GetPointer(), true);
	delete sqlDialog;

	String *sqlMessage = new String(L"DELETE FROM messages");
	r = __database->ExecuteSql(sqlMessage->GetPointer(), true);
	delete sqlMessage;

	String *sqlAttachment = new String(L"DELETE FROM attachments");
	r = __database->ExecuteSql(sqlAttachment->GetPointer(), true);
	delete sqlAttachment;

	String *sqlContacts = new String(L"DELETE FROM contacts");
	r = __database->ExecuteSql(sqlContacts->GetPointer(), true);
	delete sqlContacts;

	String *sqlRel1 = new String(L"DELETE FROM m_to_a_relations");
	r = __database->ExecuteSql(sqlRel1->GetPointer(), true);
	delete sqlRel1;

	String *sqlRel2 = new String(L"DELETE FROM m_to_fm_relations");
	r = __database->ExecuteSql(sqlRel2->GetPointer(), true);
	delete sqlRel2;

	String *sqlFwd = new String(L"DELETE FROM fwd_messages");
	r = __database->ExecuteSql(sqlFwd->GetPointer(), true);
	delete sqlFwd;

	String *sqlGeo = new String(L"DELETE FROM geo");
	r = __database->ExecuteSql(sqlGeo->GetPointer(), true);
	delete sqlGeo;
}

MDatabaseManager::~MDatabaseManager() {
	delete __database;
	__database = null;
}

Database *MDatabaseManager::GetDatabase() {
	return __database;
}

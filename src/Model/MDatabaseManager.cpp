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

	String *sqlDialog = MDialog::TableDescription();
	r = __database->ExecuteSql(sqlDialog->GetPointer(), true);
	delete sqlDialog;

	String *sqlMessage = MMessage::TableDescription();
	r = __database->ExecuteSql(sqlMessage->GetPointer(), true);
	delete sqlMessage;

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
}

MDatabaseManager::~MDatabaseManager() {
	delete __database;
	__database = null;
}

Database *MDatabaseManager::GetDatabase() {
	return __database;
}

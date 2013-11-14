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

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Io;

MDatabaseManager::MDatabaseManager() {
	String dbPath(Tizen::App::App::GetInstance()->GetAppDataPath() + L"vk.db");
	String sql;
	result r = E_SUCCESS;
	__database = new Database();
	r = __database->Construct(dbPath, "a+");

	AppLog("USER");
	String *sqlUser = MUser::TableDescription();
	r = __database->ExecuteSql(sqlUser->GetPointer(), true);
	delete sqlUser;
	AppLog("DIALOG");
	String *sqlDialog = MDialog::TableDescription();
	r = __database->ExecuteSql(sqlDialog->GetPointer(), true);
	delete sqlDialog;
	AppLog("FINISH");
}

MDatabaseManager::~MDatabaseManager() {
	delete __database;
	__database = null;
}

Database *MDatabaseManager::GetDatabase() {
	return __database;
}


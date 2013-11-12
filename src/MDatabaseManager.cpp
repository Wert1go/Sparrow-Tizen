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
	String *sqlUser = MDialog::TableDescription();
	r = __database->ExecuteSql(sqlUser->GetPointer(), true);
	delete sqlUser;

	String *sqlDialog = MUser::TableDescription();
	r = __database->ExecuteSql(sqlDialog->GetPointer(), true);
	delete sqlDialog;
}

MDatabaseManager::~MDatabaseManager() {
	delete __database;
	__database = null;
}

Database *MDatabaseManager::GetDatabase() {
	return __database;
}


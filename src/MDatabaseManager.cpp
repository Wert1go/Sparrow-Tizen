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
}

MDatabaseManager::~MDatabaseManager() {
	delete __database;
	__database = null;
}

Database *MDatabaseManager::GetDatabase() {
	return __database;
}


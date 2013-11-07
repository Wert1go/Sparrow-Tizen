/*
 * DataBaseManager.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: developer
 */

#include "DatabaseManager.h"

#include <FIo.h>
#include <FApp.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Io;

DatabaseManager::DatabaseManager() {
	String dbPath(Tizen::App::App::GetInstance()->GetAppDataPath() + L"vk.db");
	String sql;
	result r = E_SUCCESS;
	r = __database->Construct(dbPath, "a+");
}

DatabaseManager::~DatabaseManager() {
	delete __database;
	__database = null;
}

Database *DatabaseManager::GetDatabase() {
	return __database;
}


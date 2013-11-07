/*
 * DataBaseManager.h
 *
 *  Created on: Nov 8, 2013
 *      Author: developer
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <FIo.h>
using namespace Tizen::Io;

class DatabaseManager {
public:

	static DatabaseManager& getInstance()
    {
    	static DatabaseManager	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}


private:
	DatabaseManager();
	virtual ~DatabaseManager();
	DatabaseManager(DatabaseManager const&);              // Don't Implement
	void operator = (DatabaseManager const&);

public:
	Database *GetDatabase();

private:
	Database *__database;
};

#endif /* DATABASEMANAGER_H_ */

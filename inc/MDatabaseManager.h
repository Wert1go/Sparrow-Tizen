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

class MDatabaseManager {
public:

	static MDatabaseManager& getInstance()
    {
    	static MDatabaseManager	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}


private:
	MDatabaseManager();
	virtual ~MDatabaseManager();
	MDatabaseManager(MDatabaseManager const&);              // Don't Implement
	void operator = (MDatabaseManager const&);

public:
	Database *GetDatabase();

private:
	Database *__database;
};

#endif /* DATABASEMANAGER_H_ */

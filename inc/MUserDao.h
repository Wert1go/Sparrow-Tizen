/*
 * MUserDao.h
 *
 *  Created on: Nov 9, 2013
 *      Author: developer
 */

#ifndef MUSERDAO_H_
#define MUSERDAO_H_

#include "MUser.h"
#include <FIo.h>

using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

class MUserDao {

public:
	static MUserDao& getInstance()
    {
    	static MUserDao	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

private:
	MUserDao();
	virtual ~MUserDao();
	MUserDao(MUserDao const&);              // Don't Implement
	void operator = (MUserDao const&);

public:
	void Save(MUser *user);
	void Save(IList *users);

	MUser *GetCurrentUserN();
	MUser *GetUserN(int uid);
	LinkedList *GetUsersByTypeN(int type);

	DbStatement * CreateSaveStatment();
	DbStatement * BindUserToSQLStatement(MUser *user, DbStatement *statement);

	MUser * LoadUserFromDBN(DbEnumerator* pEnum);


};

#endif /* MUSERDAO_H_ */

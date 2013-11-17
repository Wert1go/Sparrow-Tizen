/*
 * MDialogDao.h
 *
 *  Created on: Nov 13, 2013
 *      Author: developer
 */

#ifndef MDIALOGDAO_H_
#define MDIALOGDAO_H_

#include "MDialog.h"
#include <FIo.h>

using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

class MMessage;
class MUser;

class MDialogDao {
public:
	static MDialogDao& getInstance()
    {
    	static MDialogDao	instance;
    	return instance;
	}

private:
	MDialogDao();
	virtual ~MDialogDao();
	MDialogDao(MDialogDao const&);
	void operator = (MDialogDao const&);

public:

	void Save(MMessage *message, MUser *user);
	void Save(MDialog *dialog);
	void Save(IList *dialogs);

	MDialog *GetDialogN(int did);
	LinkedList *GetDialogsWithOffsetN(int offset);

	DbStatement * CreateSaveStatement();
	DbStatement * BindDialogToSQLStatement(MDialog *dialog, DbStatement *statement);

	MDialog * LoadDialogFromDBN(DbEnumerator* pEnum);

	void UpdateDialogOnlineStatusById(int value, int userId);

};

#endif /* MDIALOGDAO_H_ */

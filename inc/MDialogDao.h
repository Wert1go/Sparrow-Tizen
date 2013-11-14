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

class MDialogDao {
public:
	static MDialogDao& getInstance()
    {
    	static MDialogDao	instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    	return instance;
	}

private:
	MDialogDao();
	virtual ~MDialogDao();
	MDialogDao(MDialogDao const&);              // Don't Implement
	void operator = (MDialogDao const&);

public:

	void Save(MDialog *dialog);
	void Save(IList *dialogs);

	MDialog *GetDialogN(int did);
	LinkedList *GetDialogsWithOffsetN(int offset);

	DbStatement * CreateSaveStatement();
	DbStatement * BindDialogToSQLStatement(MDialog *dialog, DbStatement *statement);

	MDialog * LoadDialogFromDBN(DbEnumerator* pEnum);

};

#endif /* MDIALOGDAO_H_ */

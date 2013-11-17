/*
 * MMessageDao.h
 *
 *  Created on: Nov 16, 2013
 *      Author: developer
 */

#ifndef MMESSAGEDAO_H_
#define MMESSAGEDAO_H_

#include <FIo.h>
#include "MMessage.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

class MMessageDao {
public:
	static MMessageDao& getInstance()
    {
    	static MMessageDao	instance;
    	return instance;
	}

private:
	MMessageDao();
	virtual ~MMessageDao();
	MMessageDao(MMessageDao const&);
	void operator = (MMessageDao const&);

public:
	void Save(MMessage *message);
	void Save(IList *messages);

	MMessage *GetMessageN(int mid);
	LinkedList *GetMessagesForUser(int userId, int lastMessageId = -1);

	DbStatement * CreateSaveStatement();
	DbStatement * BindMessageToSQLStatement(MMessage *dialog, DbStatement *statement);

	MMessage * LoadMessageFromDBN(DbEnumerator* pEnum);
};

#endif /* MMESSAGEDAO_H_ */

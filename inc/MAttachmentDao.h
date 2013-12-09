/*
 * MAttachmentDao.h
 *
 *  Created on: Nov 24, 2013
 *      Author: developer
 */

#ifndef MATTACHMENTDAO_H_
#define MATTACHMENTDAO_H_

#include <FIo.h>
#include "MAttachment.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

class MAttachmentDao {
public:
	static MAttachmentDao& getInstance()
    {
    	static MAttachmentDao	instance;
    	return instance;
	}

private:
	MAttachmentDao();
	virtual ~MAttachmentDao();
	MAttachmentDao(MAttachmentDao const&);
	void operator = (MAttachmentDao const&);

public:
	DbStatement * CreateSavePhotoStatement();
	DbStatement * BindPhotoToSQLStatement(MAttachment *photo, DbStatement *statement);
	MAttachment * LoadPhotoFromDBN(DbEnumerator* pEnum);

	DbStatement * CreateSaveVideoStatement();
	DbStatement * BindVideoToSQLStatement(MAttachment *attach, DbStatement *statement);
	MAttachment * LoadVideoFromDBN(DbEnumerator* pEnum);

	void SaveAttachments(IList *pAttachments, int mid);
	LinkedList * GetAttachments(int mid);
};

#endif /* MATTACHMENTDAO_H_ */

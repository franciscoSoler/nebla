/*
 * FileLock.h
 *
 *  Created on: 19/04/2012
 *      Author: alex
 */

#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../common/Object.h"

/**
 *
 */
class LockFile: public Object
{
public:
	typedef enum {READ_LOCK=0,WRITE_LOCK}LockTypeEnum;
private:
	struct flock m_flStruct;
	int m_fd;
	char m_fileName [ 255 ];

public:

	/**
	 *
	 */
	LockFile (std::string file_name);

	/**
	 *
	 */
	virtual ~LockFile();

	/**
	 *
	 */
	void lock (LockTypeEnum lock_type=WRITE_LOCK);

	/**
	 *
	 */
	bool isLocked();

	/**
	 *
	 */
	void release ();

	/**TODO testear
	 *
	 */
	void write (const char* buffer,int buffsize );

	/**TODO testear
	 *
	 */
	void read ( char* buffer,int buffsize );
};

#endif /* FILELOCK_H_ */

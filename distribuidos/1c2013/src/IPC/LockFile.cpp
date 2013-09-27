/*
 * FileLock.cpp
 *
 *  Created on: 19/04/2012
 *      Author: alex
 */

#include "LockFile.h"
#include <exception>

LockFile :: LockFile ( std::string file_name)
{
	int tempFd;
	strcpy ( this->m_fileName,file_name.c_str());

	this->m_flStruct.l_type = F_WRLCK;
	this->m_flStruct.l_whence = SEEK_SET;
	this->m_flStruct.l_start = 0;
	this->m_flStruct.l_len = 0;
	this->m_flStruct.l_pid = getpid ();
	tempFd = open ( this->m_fileName,O_CREAT|O_WRONLY|O_APPEND,0777 );

	if(tempFd < 0)
		throw new std::exception;

	this->m_fd = tempFd;
}



void LockFile :: lock (LockTypeEnum lock_type)
{
	switch(lock_type)
	{
	case WRITE_LOCK:
		this->m_flStruct.l_type = F_WRLCK;
		break;
	case READ_LOCK:
		this->m_flStruct.l_type = F_RDLCK;
		break;
	}

	if( fcntl ( this->m_fd,F_SETLKW,&(this->m_flStruct) ) < 0)
		throw new std::exception;
}

void LockFile :: release ()
{
	this->m_flStruct.l_type = F_UNLCK;

	if(fcntl ( this->m_fd,F_SETLK,&(this->m_flStruct) ) < 0)
		throw new std::exception;
}

void LockFile :: write ( const char* buffer,int buffsize )
{
	if(this->m_flStruct.l_type != F_WRLCK)
		throw new std::exception;

	if(lseek ( this->m_fd,0,SEEK_END ) < 0)
		throw new std::exception;

	if( ::write ( this->m_fd,buffer,buffsize ) < 0)
		throw new std::exception;
}

bool LockFile::isLocked()
{
	bool retVal;

	if(fcntl ( this->m_fd,F_GETLK,&(this->m_flStruct) ) < 0)
		throw new std::exception;

	if(this->m_flStruct.l_type == F_UNLCK)
		retVal = false;
	else
		retVal = true;

	return retVal;
}

void LockFile :: read ( char* buffer,int buffsize )
{
	if(lseek ( this->m_fd,0,SEEK_END ) < 0)
		throw new std::exception;

	if( ::read ( this->m_fd,buffer,buffsize ) < 0)
		throw new std::exception;
}

LockFile :: ~LockFile ()
{
	close ( this->m_fd );
}

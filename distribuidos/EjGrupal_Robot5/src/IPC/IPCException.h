/* 
 * File:   IPCException.h
 * Author: knoppix
 *
 * Created on October 31, 2013, 9:43 PM
 */

#ifndef IPCEXCEPTION_H
#define	IPCEXCEPTION_H

#include <exception>

class IPCException : public std::exception {
private:
	char description[255];

public:
	IPCException(const char *className, const char *error);

        const char* what() const throw();
};

#endif	/* IPCEXCEPTION_H */


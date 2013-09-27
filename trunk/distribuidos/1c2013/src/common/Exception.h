/*
 * Exception.h
 *
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>
#include <iostream>
#include "Object.h"

class Exception: public std::exception, public Object
{
public:
	Exception();
	Exception(std::string what);
	virtual ~Exception() throw();
	const char* what() const throw();

private:
	std::string m_what;
};

#endif /* EXCEPTION_H_ */

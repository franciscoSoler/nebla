/*
 * Exception.cpp
 *
 */

#include "Exception.h"

Exception::Exception()
{
}

Exception::Exception(std::string what)
{
	m_what = what;
}

Exception::~Exception() throw()
{
}

const char *Exception::what() const throw()
{
	return m_what.c_str();
}



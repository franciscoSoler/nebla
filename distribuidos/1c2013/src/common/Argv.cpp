/*
 * Argv.cpp
 *
 *  Created on: 29/04/2012
 *      Author: alex
 */

#include "Argv.h"

#include <iostream>
#include "StringUtils.h"

using namespace std;

Argv::Argv()
{
}

Argv::~Argv()
{
}

void Argv::pushArg(std::string arg, ElemPosition pos)
{
	if(pos == PUSH_BACK)
		m_Arguments.push_back(arg);
	else
		m_Arguments.insert(m_Arguments.begin(),arg);
}

void Argv::pushArg(int arg, ElemPosition pos)
{
	pushArg(StringUtils::NumberToString<int>(arg),pos);
}

void Argv::setSerializedArgs(std::string args)
{
	m_Arguments.clear();
	m_Arguments = StringUtils::splitString(args, ' ');
}

void Argv::clear()
{
	m_Arguments.clear();
}

int Argv::getArgc()
{
	return m_Arguments.size();
}

char * const*Argv::getArgv()
{
	ArgumentList::iterator it;

	m_Argv.clear();

	for(it=m_Arguments.begin(); it != m_Arguments.end(); it++)
		m_Argv.push_back(it->c_str());

	m_Argv.push_back(NULL);

	return (char* const*)&m_Argv[0];
}

void Argv::setArgv(char*const* argv)
{
	std::string item;
	char *const*p = argv;
	int i=0;

	m_Arguments.clear();

	while(*p !=NULL)
	{
		item = *p;
		m_Arguments.push_back(item);
		i++;
		p = argv+i;
	}
}

std::string Argv::serializeArgs()
{
	ArgumentList::iterator it;
	std::string retStr ="";

	for(it = m_Arguments.begin(); it != m_Arguments.end(); it++)
	{
		retStr += *it + " ";
	}
	return retStr;
}

/**
 *
 */
std::string Argv::getArg(unsigned int n)
{
	if(n >= m_Arguments.size())
		throw new std::exception;

	return m_Arguments[n];
}

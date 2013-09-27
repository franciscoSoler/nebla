/*
 * ProcessAux.h
 *
 */

#ifndef PROCESSAUX_H_
#define PROCESSAUX_H_
#include <iostream>
#include <Argv.h>
#include <list>

typedef std::list<pid_t> pidList_t;

class ProcessAux
{
public:
	static pidList_t m_PidList;
public:
	static void launchProcess(std::string procPath,int numberOfProc, Argv argv);
};

#endif /* PROCESSAUX_H_ */

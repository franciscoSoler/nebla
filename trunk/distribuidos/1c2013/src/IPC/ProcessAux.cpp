/*
 * ProcessAux.cpp
 *
 */

#include "ProcessAux.h"
#include <iostream>
#include <cstdlib>

pidList_t ProcessAux::m_PidList;

void ProcessAux::launchProcess(std::string procPath,int numberOfProc, Argv arguments)
{

    for(int i=0;i<numberOfProc;i++)
    {
    	Argv current_arguments = arguments;
    	current_arguments.pushArg(i, Argv::PUSH_FRONT);
    	current_arguments.pushArg(procPath,Argv::PUSH_FRONT);

        pid_t pid = fork();
        if(pid <0)
        {
            std::cout << "Error al iniciar el productor"<<std::endl;
            exit(1);
        }
        else
        {
            if(!pid)
            {
                if(execve(procPath.c_str(),current_arguments.getArgv(),NULL))
                {
                    std::cout << "Error al cargar el codigo del productor"<<std::endl;
                    exit(1);
                }
            }
            else
            	m_PidList.push_back(pid);
        }
    }
}

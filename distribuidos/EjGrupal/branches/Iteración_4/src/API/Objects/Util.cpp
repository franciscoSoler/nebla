#include "Util.h"
#include <stdlib.h>
#include <time.h>
#include <Logger/Logger.h>
#include <string.h>
#include <errno.h>
// #include <stdlib.h>
#include <list>
#include <stdio.h>

#define MAX_PARAMS_SIZE     200

char Util::buffer[] = {0};

Util & Util::getInstance() {
	/* Variable instantiated in the first use, destroyed 
	 *  automatically in the end of the program
	 */
    
    static Util m_Instance;
	return m_Instance;
}

long Util::generateRandomNumber(long minValue, long maxValue) {
    srand( time(NULL) + getpid() );
     return minValue + rand() % ( (abs(maxValue - minValue) + 1) );
}

void Util::createProcess(std::string processName, 
    int amountOfProcesses, int parameterOffset) {
    pid_t pid;
    static char param1[20];

    for (int i = parameterOffset; i < amountOfProcesses + parameterOffset; i++) {
        sprintf(param1, "%d", i);
        sprintf(buffer, "./%s", processName.c_str());
        
        if ((pid = fork()) < 0) {
            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
        }
        else if (pid == 0) {
            // Child process. Pass the arguments to the process and call exec
            execlp(buffer, processName.c_str(), param1, (char *) 0);

            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);

            return;
        }
    }
    
}

void Util::createProcess(std::string processName,
                         std::string arg1,
                         std::string arg2) {
    pid_t pid;
    static char name[30];
    static char param1[30];
    static char param2[30];

    sprintf(name, "./%s", processName.c_str());
    strcpy(param1, arg1.c_str());
    strcpy(param2, arg2.c_str());

    if ((pid = fork()) < 0) {
        sprintf(name, "%s Error: %s", processName.c_str(), strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
    }
    else if (pid == 0) {
        // Child process. Pass the arguments to the process and call exec
        execlp(name, processName.c_str(), param1, param2, (char *) 0);

        sprintf(name, "%s Error: %s", processName.c_str(), strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);

        return;
    }
}

void Util::createProcess(std::string processName,
                         std::string arg1,
                         std::string arg2,
                         std::string arg3) {
    pid_t pid;
    static char name[30];
    static char param1[30];
    static char param2[30];
    static char param3[30];

    sprintf(name, "./%s", processName.c_str());
    strcpy(param1, arg1.c_str());
    strcpy(param2, arg2.c_str());
    strcpy(param3, arg3.c_str());

    if ((pid = fork()) < 0) {
        sprintf(name, "%s Error: %s", processName.c_str(), strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
    }
    else if (pid == 0) {
        // Child process. Pass the arguments to the process and call exec
        execlp(name, processName.c_str(), param1, param2, param3, (char *) 0);

        sprintf(name, "%s Error: %s", processName.c_str(), strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);

        return;
    }
}


Util::Util(const Util& orig) {
}

Util::~Util() {
}


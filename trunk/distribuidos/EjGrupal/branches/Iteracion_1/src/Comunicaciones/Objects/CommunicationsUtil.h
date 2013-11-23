#ifndef COMMUNICATIONSUTIL_H
#define COMMUNICATIONSUTIL_H

#include <Logger/Logger.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <list>
#include <unistd.h>
#include <sys/types.h>

class CommunicationsUtil
{
public:
    CommunicationsUtil();
    int parseArgs(char argc, char* argv[], int &processID);
    int parseChannelArgs(char server[], int &inputPort, int &outputPort);
    /* Save the server pid in a plain text file named RegisteredServers.txt */
    void registerServer(pid_t serverPid);
    std::list<pid_t> getRegisteredServers();

private:
    // Used in logging operations
    char buffer_[255];
};

#endif // COMMUNICATIONSUTIL_H

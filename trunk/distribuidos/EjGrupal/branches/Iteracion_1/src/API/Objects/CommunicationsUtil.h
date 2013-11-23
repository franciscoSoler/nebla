#ifndef COMMUNICATIONSUTIL_H
#define COMMUNICATIONSUTIL_H

#include <Logger/Logger.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

class CommunicationsUtil
{
public:
    CommunicationsUtil();
    int parseArgs(char argc, char* argv[], int &processID);
    int parseChannelArgs(char server[], int &inputPort, int &outputPort);

private:
    // Used in logging operations
    char buffer_[255];
};

#endif // COMMUNICATIONSUTIL_H

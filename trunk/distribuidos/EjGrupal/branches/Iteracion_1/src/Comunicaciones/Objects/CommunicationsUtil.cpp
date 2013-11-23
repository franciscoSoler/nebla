#include "CommunicationsUtil.h"
#include <sstream>
#include <string>
#include <fstream>
#include <Common.h>
#include <Logger/LockFile.h>


CommunicationsUtil::CommunicationsUtil() {
}

int CommunicationsUtil::parseArgs(char argc, char* argv[], int &processID) {
    if (argc != 2) {
        Logger::getInstance().setProcessInformation("Canal Entrada Cliente");
        Logger::logMessage(Logger::ERROR, "Error: Cantidad de parametros invalida.");
        return -1;
    }

    sscanf(argv[1], "%d", &processID);
    return 0;
}

int CommunicationsUtil::parseChannelArgs(char server[], int &inputPort, int &outputPort) {
    std::ifstream input("Config", std::ifstream::in);
    if (!input) {
        Logger::logMessage(Logger::ERROR, "Error al abrir archivo de configuración");
        return -1;
    }

    // FIXME: Replace this with any Parser
    input >> server;
    input >> inputPort;
    input >> outputPort;
    input.close();

    return 0;
}

void CommunicationsUtil::registerServer(pid_t serverPid) {
    std::stringstream ss;
    ss << serverPid << std::endl;

    LockFile lock(SERVERS_FILE_NAME);
    lock.takeLock();
    lock.writeToFile(ss.str().c_str(), ss.str().length());
    lock.releaseLock();
}

std::list<pid_t> CommunicationsUtil::getRegisteredServers() {
    pid_t serverPid;
    std::ifstream input(SERVERS_FILE_NAME, std::ifstream::in);
    std::list<pid_t> serverList;

    while( input >> serverPid ) {
        serverList.push_back(serverPid);
    }

    input.close();
    return serverList;
}

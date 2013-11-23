#include "CommunicationsUtil.h"

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

int CommunicationsUtil::parseChannelArgs(char server[], int & inputPort, int &outputPort) {
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

#include "ServersManager.h"
#include <Logger/Logger.h>
#include <Logger/LockFile.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <signal.h>
#include <ConfigFileParser/ConfigFileParser.h>
#include <Socket/SocketConnector.h>
#include <memory>
#include <Common.h>
#include <API/Objects/Util.h>


ServersManager::ServersManager() {
}

void ServersManager::createServer(const char* serverName) {
    Util::getInstance();
    pid_t pid;
    std::string processName = serverName;

    if ((pid = fork()) < 0) {
        sprintf(buffer, "%s Error: %s", serverName, strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
    }
    else if (pid == 0) {
        // Register server pid
        sprintf(buffer, "Creando Server - Pid %d", getpid());
        Logger::logMessage(Logger::IMPORTANT, buffer);
        this->registerServer( getpid() );

        execlp("./ConcurrentServer", "ConcurrentServer", processName.c_str(), (char *) 0);
        sprintf(buffer, "%s Error: %s", "ConcurrentServer", strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
        return;
    }
}

void ServersManager::createBrokerServer(const char* brokerServerName, int brokerId) {
    pid_t pid;
    std::stringstream ss;
    std::string processName = brokerServerName;
    std::string processId;
    ss << brokerId;
    ss >> processId;

    if ((pid = fork()) < 0) {
        sprintf(buffer, "%s Error: %s", brokerServerName, strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
    }
    else if (pid == 0) {
        // Register server pid
        sprintf(buffer, "Creando Server - Pid %d", getpid());
        Logger::logMessage(Logger::IMPORTANT, buffer);
        this->registerServer( getpid() );

        execlp("./BrokerConcurrentServer", "BrokerConcurrentServer",
               processName.c_str(), processId.c_str(), (char *) 0);
        sprintf(buffer, "%s Error: %s", "BrokerConcurrentServer",
                strerror(errno));
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
        return;
    }
}

void ServersManager::createChannelToBroker(const char* brokerChannelName,
                                           int brokerId,
                                           int remoteBrokerId) {
    std::stringstream ssBrokerId;
    std::stringstream ssRemoteBrokerId;
    ssBrokerId << brokerId;
    ssRemoteBrokerId << remoteBrokerId;
    std::string socketNumber = "0";

    Util::createProcess(brokerChannelName,
                        socketNumber,
                        ssBrokerId.str(),
                        ssRemoteBrokerId.str());
}

SocketStream* ServersManager::connectToServer(const char* serverName) {
    std::auto_ptr<IConfigFileParser> configFileParser(
                new ConfigFileParser(SERVERS_CONFIG_FILE) );
    configFileParser->parse();

    std::string server = serverName;

    // First, obtain the parameters
    std::string serverDir = configFileParser->getConfigFileParam(
                server + "-Direccion", "");
    int puerto = configFileParser->getConfigFileParam(server + "-Port", -1);

    if (puerto == -1 || serverDir.empty() ) {
        Logger::logMessage(Logger::COMM, "connectToServer(): Error en la carga de parámetros");
    }

    // Create the connection
    SocketConnector connector;
    SocketStream* canal = connector.connect(puerto, serverDir.c_str());

    return canal;
}

SocketStream* ServersManager::connectToBrokerServer(const char* serverName, int brokerNumber) {
    std::auto_ptr<IConfigFileParser> configFileParser(
                new ConfigFileParser(SERVERS_CONFIG_FILE) );
    configFileParser->parse();

    std::stringstream ss;
    ss << brokerNumber;
    std::string server = std::string(serverName) + "-" + ss.str();

    // First, obtain the parameters
    std::string serverDir = configFileParser->getConfigFileParam(
                server + "-Direccion", "");
    int puerto = configFileParser->getConfigFileParam(server + "-Port", -1);

    if (puerto == -1 || serverDir.empty() ) {
        Logger::logMessage(Logger::COMM, "connectToBrokerServer(): Error en la carga de parámetros");
    }

    // Create the connection
    SocketConnector connector;

    SocketStream* canal = connector.connect(puerto, serverDir.c_str());

    return canal;
}


void ServersManager::registerServer(pid_t serverPid) {
    std::stringstream ss;
    ss << serverPid << std::endl;

    LockFile lock(REGISTRATION_FILE);
    lock.takeLock();
    lock.writeToFile(ss.str().c_str(), ss.str().length());
    lock.releaseLock();
}

std::list<pid_t> ServersManager::getRegisteredServers() {
    pid_t serverPid;
    std::ifstream input(REGISTRATION_FILE, std::ifstream::in);
    std::list<pid_t> serverList;

    while( input >> serverPid ) {
        serverList.push_back(serverPid);
    }

    input.close();

    remove(REGISTRATION_FILE);
    return serverList;
}


void ServersManager::killServers() {
    std::list<pid_t> serverList = this->getRegisteredServers();
    std::list<pid_t>::iterator it;
    char buffer[255];

    for (it = serverList.begin(); it != serverList.end(); it++) {
        pid_t serverPid = *it;

        sprintf(buffer, "Matando Server con Pid %d", serverPid);
        Logger::logMessage(Logger::IMPORTANT, buffer);

        kill(serverPid, SIGINT);
    }
}

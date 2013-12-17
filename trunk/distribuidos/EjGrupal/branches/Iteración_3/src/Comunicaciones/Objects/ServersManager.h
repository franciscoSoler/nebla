#ifndef SERVERSMANAGER_H
#define SERVERSMANAGER_H

/* This class will be used for Programs that want to create Concurrent Servers,
 * obtaining information of them from a configuration file.
 * Also, it will be used for clients that want to interact with that
 * servers, creating the communication channels between them.
 */

#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <Socket/SocketStream.h>

#define REGISTRATION_FILE               "RegisteredServers.txt"

class ServersManager {
public:
    ServersManager();

    /* The data of the serverName used has to exists in
     * the configuration file
     */
    void createServer(const char* serverName, int brokerNumber);

    /* The broker has to exists int the configuration file and the brokerId
     * one of the Brokers in the same file
     */
    void createBrokerServer(const char* brokerServerName, int brokerId);

    /* The broker has to be up so the channel is formed
     */
    void createChannelToBroker(const char* brokerServerName,
                               int brokerId,
                               int removeBrokerId);

    /* The processHandler has to exists as a program */
    SocketStream* connectToServer(const char* serverName);

    /* The processHandler has to exists as a program */
    SocketStream* connectToBrokerServer(const char* serverName, int brokerNumber);

    /* Execute a kill system call to all
     * the servers registered in the REGISTRATION_FILE
     */
    void killServers(int brokerNumber);

private:
    /* Save the server pid in a plain text file named RegisteredServers.txt */
    void registerServer(pid_t serverPid, int brokerNumber);
    std::list<pid_t> getRegisteredServers(int brokerNumber);
    void createProcess(std::string processName,
                         std::string arg1,
                         std::string arg2,
                         std::string arg3);

private:
    char buffer[255];
};

#endif // SERVERSMANAGER_H

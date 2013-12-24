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
    void createServer(const char* serverName);
    /* The processHandler has to exists as a program */
    SocketStream* connectToServer(const char* serverName);
    /* */
    void killServers();

private:
    /* Save the server pid in a plain text file named RegisteredServers.txt */
    void registerServer(pid_t serverPid);
    std::list<pid_t> getRegisteredServers();

private:
    char buffer[255];
};

#endif // SERVERSMANAGER_H

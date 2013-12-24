#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <Common.h>
#include <Logger/Logger.h>
#include <ConfigFileParser/ConfigFileParser.h>

#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Signals/SignalHandler.h>
#include <Comunicaciones/Signals/Handlers/SIGINT_Handler.h>
#include <Socket/SocketAcceptor.h>
#include <Socket/SocketStream.h>


int main(int argc, char **argv) {
    std::string serverName;
    char buffer[TAM_BUFFER];
    CommunicationsUtil util;
    std::auto_ptr<IConfigFileParser> configFileParser(
                new ConfigFileParser(SERVERS_CONFIG_FILE) );

    if ( util.parseArgs(argc, argv, serverName) == -1 ) {
        exit(-1);
    }
    Logger::setProcessInformation(serverName + ":");


    /* string que contiene el socket para el servidor de eco */
    static char dispatcherSocketChar[15];


    /* Se registra el handler para la signal SIGINT, para reaccionar
     * adecuadamente cuando intenten matar al Server
     */
    SIGINT_Handler handler;
    SignalHandler::getInstance().registerHandler(SIGINT, &handler);


    /* Se obtienen parámetros de configuración del server */
    configFileParser->parse();
    int puerto = configFileParser->getConfigFileParam(serverName + "-Port", -1);
    std::string processDispatcher = configFileParser->getConfigFileParam(
                serverName + "-Dispatcher", "");

    if (puerto == -1 || processDispatcher.empty() ) {
        Logger::logMessage(Logger::ERROR,
        "Se produjo un error al cargar los parámetros de configuración del server");
        abort();
    }


    sprintf(buffer, "Levantando server: %s en puerto %d.", serverName.c_str(), puerto);
    Logger::logMessage(Logger::COMM, buffer);


    /* Se configura el server para que empiece a aceptar conexiones y pueda
     * despachar las mismas
     */
    SocketAcceptor acceptor(puerto);
    acceptor.configureSocket();
    Logger::logMessage(Logger::COMM, "Comienza a escuchar conexiones de clientes");

    while ( true ) {
        SocketStream::SocketStreamPtr dispatcherSocket( acceptor.accept() );
        if (not dispatcherSocket.get() ) {
            break;
        }
        Logger::logMessage(Logger::COMM, "Acepta conexión. Procede a forkear la misma");
        sprintf(dispatcherSocketChar, "%d", dispatcherSocket->getSd());

        pid_t pid = fork();
        if (pid == 0) {
            execlp( (std::string("./") + processDispatcher).c_str() ,
                    processDispatcher.c_str(), dispatcherSocketChar, (char *) 0);

            sprintf(buffer, "execlp ./%s error: %s", processDispatcher.c_str(), strerror(errno));
            Logger::logMessage(Logger::ERROR, buffer);
        }
        // No se si esto está bien, por el momento funciona (en OS Debian Wheezy - 64 bits)
        dispatcherSocket->destroy();
    }

    acceptor.destroy();
    return 0;
}

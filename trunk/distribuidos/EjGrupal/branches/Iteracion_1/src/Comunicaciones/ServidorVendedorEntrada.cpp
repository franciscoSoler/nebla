
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "../Common.h"
#include "../Socket/inet.h"

#include "../Logger/Logger.h"

extern int tcpoppas(int puerto);

int main(int argc, char **argv) {

    Logger::setProcessInformation("Servidor Vendedor Entrada");
    
    static char socketEntradaChar[15]; /* string que contiene el socket para el servidor de eco */

    char buffer[TAM_BUFFER];
    char server[TAM_BUFFER]; // Unused

    int puertoEntrada = 0;

    std::ifstream input("Config", std::ifstream::in);
    if (!input) {
        Logger::logMessage(Logger::ERROR,"Error al abrir el archivo de configuracion.");
        exit(1);
    }
    input >> server;
    input >> puertoEntrada;
    input.close();

    sprintf(buffer, "Levantando server: %s en puerto entrada %d.", server, puertoEntrada);
    Logger::logMessage(Logger::TRACE, buffer);

    int socketEntradaPasivo;
    int socketEntrada; /* socket conectado al cliente */
    unsigned clilen; /* longitud dir. cliente */
    struct sockaddr_in cli_addr;

    if ((socketEntradaPasivo = tcpoppas(puertoEntrada)) < 0) {
        Logger::logMessage(Logger::ERROR, "No se puede abrir el stream socket (error en open pasivo)");
        exit(1);
    }

    sprintf(buffer, "Se hizo el open pasivo, socket %d\n", socketEntradaPasivo);
    Logger::logMessage(Logger::TRACE, buffer);
    
    while (true) {
        clilen = sizeof (cli_addr);
        socketEntrada = accept(socketEntradaPasivo, (struct sockaddr *) &cli_addr, &clilen);
        if (socketEntrada < 0) {
            Logger::logMessage(Logger::ERROR, "Error en el accept");
            exit(1);
        }
        sprintf(socketEntradaChar, "%d\n", socketEntrada); /* Pasarle el socket al hijo que atiende */

        pid_t pid = fork();
        if (pid == 0) {
            close(socketEntradaPasivo);
            execlp("./canalEntradaVendedor", "canalEntradaVendedor", socketEntradaChar, (char *) 0);
            sprintf(buffer, "Execlp ./canalEntradaVendedor error: %s\n", strerror(errno));
            Logger::logMessage(Logger::ERROR, buffer);
        }
    }
    return 0;
} 

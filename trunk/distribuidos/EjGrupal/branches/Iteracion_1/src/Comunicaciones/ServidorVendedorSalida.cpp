
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "../Common.h"
#include "../Socket/inet.h"

#include "../Logger/Logger.h"

extern int tcpoppas(int puerto);

int main(int argc, char **argv) {

    Logger::setProcessInformation("Servidor Vendedor Salida");

    static char socketSalidaChar[15];

    char buffer[TAM_BUFFER];

    char server[TAM_BUFFER]; // Unused
    int puertoEntrada = 0; // Unused
    int puertoSalida = 0;

    std::ifstream input("Config", std::ifstream::in);
    if (!input) {
        Logger::logMessage(Logger::ERROR, "Error al abrir el archivo de configuracion.");
        exit(1);
    }
    input >> server;
    input >> puertoEntrada;
    input >> puertoSalida;
    input.close();

    sprintf(buffer, "Levantando server: %s en puerto salida %d", server, puertoSalida);
    Logger::logMessage(Logger::COMM, buffer);

    int socketSalidaPasivo;
    int socketSalida; /* socket conectado al cliente */
    unsigned clilen; /* longitud dir. cliente */
    struct sockaddr_in cli_addr;

    if ((socketSalidaPasivo = tcpoppas(puertoSalida)) < 0) {
        Logger::logMessage(Logger::ERROR, "No se puede abrir el stream socket (error en open pasivo)");
        exit(1);
    }
    
    sprintf(buffer, "Se hizo el open pasivo, socket %d", socketSalidaPasivo);
    Logger::logMessage(Logger::COMM, buffer);
    
    while (true) {
        clilen = sizeof (cli_addr);

        socketSalida = accept(socketSalidaPasivo, (struct sockaddr *) &cli_addr, &clilen);
        if (socketSalida == -1) {
            Logger::logMessage(Logger::ERROR, "Error en accept. Abortando Servidor");
            abort();
        }

        Logger::logMessage(Logger::COMM, "Se conecto un cliente al socket.");

        sprintf(socketSalidaChar, "%d", socketSalida); /* Pasarle el socket al hijo que atiende */

        pid_t pid = fork();
        if (pid == 0) {
            close(socketSalidaPasivo);
            execlp("./CanalSalidaVendedor", "CanalSalidaVendedor", socketSalidaChar, (char *) 0);
            sprintf(buffer, "Execlp ./CanalSalidaVendedor error: %s", strerror(errno));
            Logger::logMessage(Logger::ERROR, buffer);
        }
    }

    return 0;
}

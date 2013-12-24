/* 
 * File:   Repositor.cpp
 * Author: fede
 *
 * Created on November 9, 2013, 5:29 PM
 */

#include <cstdlib>
#include "API/Repositor/ControladorRepositor.h"
#include "Common.h"
#include "Logger/Logger.h"

#define MAX_ESPERA_REPONEDOR 20

int main(int argc, char** argv)
{
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "Repositor de canastos de piezas.");
    Logger::setProcessInformation(buffer);
    
    if(argc < 2) {
	Logger::logMessage(Logger::ERROR, "Cantidad de parametros invalida. Uso: ./RepositorCanastos [TIPO_PIEZA]");
	exit(-1);
    }
    
    int numeroPieza = atoi(argv[1]);
    ControladorRepositor controlador;
    controlador.iniciarControlador();
    
    sprintf(buffer, "Reponiendo pieza %d.", numeroPieza);
    Logger::logMessage(Logger::TRACE, buffer);

    int demoraReposicion = rand() % MAX_ESPERA_REPONEDOR + 1;
    usleep(demoraReposicion * 1000 * 1000);
    
    controlador.reponerCanastos(numeroPieza);
    
    return 0;
}


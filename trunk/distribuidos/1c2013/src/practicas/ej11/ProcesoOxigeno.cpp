/* 
 * File:   ProcesoOxigeno.cpp
 * Author: distribuidos
 *
 * Created on April 13, 2013, 1:58 PM
 */

#include <cstdlib>
#include "Oxigeno.h"
#include "Logger.h"
#include <sstream>
#include "SorteadorEntero.h"


using namespace std;

void generarOxigeno(int);

/*
 * 
 */
int main(int argc, char** argv) {
    char buffer[256];

    if (argc != 2) {
        strcpy(buffer, "ProcesoOxigeno: Error: Cantidad de parametros invalida\n Uso:\n ./oxigeno [NRO OXIGENO] \n");
        write(fileno(stderr), buffer, strlen(buffer));
        exit(1);
    }
    int nro = 0;
    sscanf(argv[1], "%d", &nro);

    Oxigeno oxigeno(nro);
    oxigeno.iniciar();
    generarOxigeno(nro);
    oxigeno.terminar();
    return 0;
}

void generarOxigeno(int nro) {
    SorteadorEntero sorteador;
    int numeroAleatorio=sorteador.sortear(10,50);
    float tiempo= numeroAleatorio/10.0;
    
    stringstream etiqueta;
    etiqueta<< "OxigenoUsuario " <<nro;
    Logger logger(etiqueta.str());
    
    stringstream mensajeLog;
    mensajeLog<<"Generando oxigeno ( "<< tiempo<< " seg ).";
    logger.log(mensajeLog.str());
    sleep(numeroAleatorio/10.0);
    logger.log("Oxigeno generado.");
}
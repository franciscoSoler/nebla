/* 
 * File:   ProcesoHidrogeno.cpp
 * Author: distribuidos
 *
 * Created on April 13, 2013, 1:54 PM
 */

#include <cstdlib>
#include "Hidrogeno.h"
#include "Logger.h"
#include "SorteadorEntero.h"
#include <sstream>
using namespace std;

void generarHidrogeno(int);

/*
 * 
 */
int main(int argc, char** argv) {
    
    char buffer[256];

    if (argc != 2) {
        strcpy(buffer, "ProcesoHidrogeno: Error: Cantidad de parametros invalida\n Uso:\n ./hidrogeno [NRO HIDROGENO] \n");
        write(fileno(stderr), buffer, strlen(buffer));
        exit(1);
    }
    int nro = 0;
    sscanf(argv[1], "%d", &nro);
    
    Hidrogeno hidrogeno(nro);
    hidrogeno.iniciar();
    generarHidrogeno(nro);
    hidrogeno.terminar();
    return 0;
}

void generarHidrogeno(int nro){
    SorteadorEntero sorteador;
    int numeroAleatorio=sorteador.sortear(10,50);
    float tiempo= numeroAleatorio/10.0;
    
    stringstream etiqueta;
    etiqueta<< "HidrogenoUsuario " <<nro;
    Logger logger(etiqueta.str());
    
    stringstream mensajeLog;
    mensajeLog<<"Generando hidrogeno ( "<< tiempo<< " seg ).";
    logger.log(mensajeLog.str());
    
    sleep(numeroAleatorio/10.0);
    
    logger.log("Hidrogeno generado.");
}

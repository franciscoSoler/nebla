/* 
 * File:   AGV.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:30
 */
#include <sstream>
#include <memory>
#include <cstdlib>
#include "Common.h"
#include "API/AGV/IControladorAGV.h"
#include "API/AGV/ControladorAGV.h"

/*
 * 
 */
int main(int argc, char** argv) {
    std::stringstream ss;
    int nroRobot;
    ss << argv[1];
    ss >> nroRobot;
    
    std::auto_ptr<IControladorAGV> controladorAGV = std::auto_ptr<IControladorAGV>(new ControladorAGV());
    controladorAGV->iniciarControlador(nroRobot);
    while(true) {
        TipoProducto tipoPieza = controladorAGV->atenderPedidos();
        Canasto canasto = controladorAGV->buscarPieza(tipoPieza);
        controladorAGV->reponerCanasto(canasto);
    }
    return 0;
}


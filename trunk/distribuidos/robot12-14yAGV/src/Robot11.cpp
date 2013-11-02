/* 
 * File:   Robot11.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:30
 */
#include <sstream>
#include <cstdlib>
#include <memory>
#include "Common.h"
#include "API/IControladorRobot11.h"
#include "Controladores/ControladorRobot11.h"

/*
 * 
 */
int main(int argc, char** argv) {
    std::stringstream ss;
    int nroRobot;
    ss << argv[1];
    ss >> nroRobot;
    
    std::auto_ptr<IControladorRobot11> controladorRobot11 = std::auto_ptr<IControladorRobot11>(new ControladorRobot11());
    controladorRobot11->iniciarControlador(nroRobot);
    Caja unaCaja;
    EspecifProd piezas;
    
    while (true) {
        while (controladorRobot11->buscarProximoGabinete(&piezas) == false)
            controladorRobot11->avanzarCinta(); 
            
        //for (int i = 0; i < piezas.cantPiezas; i++) {
            while (true) { // anteriormente tenia tipo de pieza
                if (!controladorRobot11->agregarPantalla(piezas.tipoPantalla))
                    controladorRobot11->pedirPiezaAlAGV(piezas.tipoPantalla.tipoPieza);
                if (piezas.tipoPantalla.cantidad-- == 0)
                    break;
            }            
        //}
        unaCaja = controladorRobot11->cerrarYTomarCaja();
        controladorRobot11->depositarCaja(unaCaja);
    }
    return 0;
}


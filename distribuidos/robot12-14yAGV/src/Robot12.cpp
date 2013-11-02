/* 
 * File:   Robot12.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:29
 */
#include <sstream>
#include <cstdlib>
#include <memory>
#include "Common.h"
#include "API/IControladorRobot12.h"
#include "Controladores/ControladorRobot12.h"

/*
 * 
 */
int main(int argc, char** argv) {
    std::stringstream ss;
    int nroRobot;
    ss << argv[1];
    ss >> nroRobot;
    
    std::auto_ptr<IControladorRobot12> controladorRobot12 = std::auto_ptr<IControladorRobot12>(new ControladorRobot12());
    controladorRobot12->iniciarControlador(nroRobot);
    EspecifProd piezas;
    while (true) {
        piezas = controladorRobot12->esperarProximoGabinete(); 
        for (int i = 0; i < piezas.cantPiezas; i++) {
            while (true) {
                if (!controladorRobot12->agregarConector(piezas.pieza[i]))
                    controladorRobot12->pedirPiezaAlAGV(piezas.pieza[i].tipoPieza);
                if (piezas.pieza[i].cantidad-- == 0)
                    break;
            }
        }
        controladorRobot12->finalizarEnsamble();
    }
    return 0;
}


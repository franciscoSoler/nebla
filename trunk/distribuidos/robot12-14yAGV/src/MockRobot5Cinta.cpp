/* 
 * File:   MockRobot5.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:30
 */
#include <sstream>
#include <cstdlib>
#include <memory>
#include "Common.h"
#include "API/IControladorRobot5.h"
#include "Controladores/MockControladorRobot5.h"

/*
 * 
 */
int main(int argc, char** argv) {
    std::auto_ptr<IControladorRobot5> controladorRobot5 = std::auto_ptr<IControladorRobot5>(new MockControladorRobot5());
    controladorRobot5->iniciarControlador();
    //int i = 0;
    while (true) {
        usleep(200);
        controladorRobot5->llenarCinta();
        //i++;
    }
    return 0;
}




#include <iostream>
#include <memory>
#include <API/Robot16/ControllerRobot16.h>
#include <Objects/Util.h>

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerRobot16> controller = std::auto_ptr<IControllerRobot16>(new ControllerRobot16());
    
    while (1) {
        // Avanzo la cinta mientras no haya caja al final de la misma
        while (controller->moverCinta()) {
            sleep ( Util::getInstance().generateRandomNumber(1,2) );
        }
        
        Caja unaCaja;
        controller->tomarCaja(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,2) );
        
        controller->depositarCaja(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,2) );
    }
    
    return 0;
}


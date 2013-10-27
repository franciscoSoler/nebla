#include <iostream>
#include <memory>
#include <API/Robot14/ControllerRobot14.h>
#include <Objects/Util.h>
#include <CSO/Exceptions/Exception.h>

int main(int argc, char* argv[]) {
    std::auto_ptr<IControllerRobot14> controller = std::auto_ptr<IControllerRobot14>(new ControllerRobot14());
    
    while (1) {
        // Avanzo la cinta mientras no haya caja al final de la misma
        while ( controller->moverCinta() ) {   
             sleep( Util::getInstance().generateRandomNumber(1,3) );
        }

        Caja unaCaja;
        controller->tomarCaja(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,3) );

        controller->depositarCaja(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,3) );
    }    
}


#include <iostream>
#include <memory>
#include <sstream>
#include <API/Robot11/ControllerRobot11.h>
#include <Objects/Util.h>

int main(int argc, char* argv[]) {
    char buffer[255];
    std::stringstream ss;
    int nroCinta;
    ss << argv[1];
    ss >> nroCinta;
    std::auto_ptr<IControllerRobot11> controller = std::auto_ptr<IControllerRobot11>(new ControllerRobot11(nroCinta));
    
    while (1) {
        sleep( Util::getInstance().generateRandomNumber(1, 10) );
        Caja unaCaja = controller->tomarCaja();

        sleep ( Util::getInstance().generateRandomNumber(1, 10) );
        controller->depositarCaja(unaCaja);
    }
    
    return 0;
}

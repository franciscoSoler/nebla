#include <iostream>
#include <memory>
#include <API/Robot14/ControllerRobot14.h>
#include <API/Objects/Util.h>
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>


typedef std::auto_ptr<IControllerRobot14> autoPtrControllerRobot14;

uint elegirCinta(autoPtrControllerRobot14 & controller);
uint elegirCintaPorCantidadDeCajas(autoPtrControllerRobot14 & controller);

int main(int argc, char* argv[]) {
    autoPtrControllerRobot14 controller( new ControllerRobot14() );
    char buffer[255];
    
    while (1) {
        controller->comenzarATrabajar();
        uint nroCinta = elegirCinta(controller);

        Logger::getInstance();
        sprintf(buffer, "Cinta elegida: N°%u", nroCinta);
        Logger::logMessage(Logger::TRACE, buffer);

        // Avanzo la cinta mientras no haya caja al final de la mism
        while ( controller->moverCinta(nroCinta) ) {
             sleep( Util::getInstance().generateRandomNumber(1,3) );
        }

        Caja unaCaja;
        controller->tomarCaja(unaCaja, nroCinta);
        sleep( Util::getInstance().generateRandomNumber(1,3) );

        controller->depositarCaja(unaCaja);
        sleep( Util::getInstance().generateRandomNumber(1,3) );
    }    

    return 0;
}


uint elegirCinta(autoPtrControllerRobot14 & controller) {
    // Variable cual fue la última cinta utilizada
    uint contador = 0;
    uint cintaBloqueada = 0;

    for (uint i = 1; i <= AMOUNT_CINTA_13; ++i) {
        if ( controller->estaRobot11BloqueadoEnCinta(i) ) {
            contador++;
            cintaBloqueada = i;
        }
    }

    if (contador == 1) {
        return cintaBloqueada;
    }
    else {
        // Si ambos robots están bloqueados o desbloqueados, se desempata por
        // cantidad de cajas.
        return elegirCintaPorCantidadDeCajas(controller);
    }
}

uint elegirCintaPorCantidadDeCajas(autoPtrControllerRobot14 & controller) {
    // Por defecto, el programa empieza dándole prioridad a la cinta 1,
    // en los casos de empate
    static uint cintaConPrioridad = 1;
    uint cajasPorCinta[AMOUNT_CINTA_13];

    for (uint i = 1; i <= AMOUNT_CINTA_13; i++) {
        cajasPorCinta[i-1] = controller->obtenerCantidadDeCajasEnCinta(i);
    }

    // Sanity-Check
    if (cajasPorCinta[0] == 0 && cajasPorCinta[1] == 0) {
        Logger::logMessage(Logger::ERROR, "Cintas vacías. Robot14 fue despertado erroneamente");
        abort();
    }

    if (cajasPorCinta[0] > cajasPorCinta[1]) {
        return 1;
    }
    else if (cajasPorCinta[1] > cajasPorCinta[0]) {
        return 2;
    }
    else {
        uint cintaElegida = cintaConPrioridad;
        if (cintaElegida == 1) {
            cintaConPrioridad = 2;
        }
        else if (cintaElegida == 2) {
            cintaConPrioridad = 1;
        }

        return cintaElegida;
    }
}

#include "StateR16_Durmiendo.h"
#include <CSO/Exceptions/Exception.h>

StateR16_Durmiendo::StateR16_Durmiendo(ControllerRobot16* controlador) 
: controlador_(controlador) {}

bool StateR16_Durmiendo::moverCinta() {
    return controlador_->moverCintaEnEstadoDurmiendo();
}

bool StateR16_Durmiendo::tomarCaja(Caja & unaCaja) {
    throw Exception("Acción Inválida. Estado:R16_Durmiendo - Acción:tomarCaja");
}

void StateR16_Durmiendo::depositarCaja(Caja unaCaja) {
    throw Exception("Acción Inválida. Estado:R16_Durmiendo - Acción:depositarCaja"); 
}

StateR16_Durmiendo::~StateR16_Durmiendo() {
}


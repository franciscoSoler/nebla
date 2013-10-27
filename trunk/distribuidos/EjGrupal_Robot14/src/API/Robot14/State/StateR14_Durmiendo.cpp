#include "StateR14_Durmiendo.h"
#include <CSO/Exceptions/Exception.h>

StateR14_Durmiendo::StateR14_Durmiendo(ControllerRobot14* controlador) 
: controlador_(controlador) {}

bool StateR14_Durmiendo::moverCinta() {
    return controlador_->moverCintaEnEstadoDurmiendo();
}

bool StateR14_Durmiendo::tomarCaja(Caja & unaCaja) {
    throw Exception("Acción Inválida. Estado:R14_Durmiendo - Acción:tomarCaja");
}

void StateR14_Durmiendo::depositarCaja(Caja unaCaja) {
    throw Exception("Acción Inválida. Estado:R14_Durmiendo - Acción:depositarCaja"); 
}

StateR14_Durmiendo::~StateR14_Durmiendo() {
}


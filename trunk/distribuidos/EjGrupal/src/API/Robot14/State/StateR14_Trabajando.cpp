#include "StateR14_Trabajando.h"
#include <Exceptions/Exception.h>

StateR14_Trabajando::StateR14_Trabajando(ControllerRobot14* controlador) 
: controlador_(controlador) {}

bool StateR14_Trabajando::moverCinta() {
    return controlador_->moverCintaEnEstadoTrabajando();
}

bool StateR14_Trabajando::tomarCaja(Caja & unaCaja) {
    return controlador_->tomarCajaEnEstadoTrabajando(unaCaja);
}

void StateR14_Trabajando::depositarCaja(Caja unaCaja) {
    throw Exception("Acción Inválida. Estado:R14_Trabajando - Acción:depositarCaja"); 
}

StateR14_Trabajando::~StateR14_Trabajando() {
}


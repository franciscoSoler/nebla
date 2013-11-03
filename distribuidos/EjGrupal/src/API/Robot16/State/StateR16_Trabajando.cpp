#include "StateR16_Trabajando.h"
#include <Exceptions/Exception.h>

StateR16_Trabajando::StateR16_Trabajando(ControllerRobot16* controlador) 
: controlador_(controlador) {}

bool StateR16_Trabajando::moverCinta() {
    return controlador_->moverCintaEnEstadoTrabajando();
}

bool StateR16_Trabajando::tomarCaja(Caja & unaCaja) {
    return controlador_->tomarCajaEnEstadoTrabajando(unaCaja);
}

void StateR16_Trabajando::depositarCaja(Caja unaCaja) {
    throw Exception("Acción Inválida. Estado:R16_Trabajando - Acción:depositarCaja"); 
}

StateR16_Trabajando::~StateR16_Trabajando() {
}


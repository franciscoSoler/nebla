#include "StateR16_ConCarga.h"
#include <Exceptions/Exception.h>

StateR16_ConCarga::StateR16_ConCarga(ControllerRobot16* controlador) 
: controlador_(controlador) {}

bool StateR16_ConCarga::moverCinta() {
    throw Exception("Acción Inválida. Estado:Robot16_ConCarga Accion:moverCinta"); 
}

bool StateR16_ConCarga::tomarCaja(Caja & unaCaja) {
    throw Exception("Acción Inválida. Estado:Robot16_ConCarga Accion:tomcarCaja"); 
}

void StateR16_ConCarga::depositarCaja(Caja unaCaja) {
    controlador_->depositarCajaEnEstadoConCarga(unaCaja);
}

StateR16_ConCarga::~StateR16_ConCarga() {
}


#include "StateR14_ConCarga.h"
#include <Exceptions/Exception.h>

StateR14_ConCarga::StateR14_ConCarga(ControllerRobot14* controlador) 
: controlador_(controlador) {}

bool StateR14_ConCarga::moverCinta() {
    throw Exception("Acción Inválida. Estado:Robot14_ConCarga Accion:moverCinta"); 
}

bool StateR14_ConCarga::tomarCaja(Caja & unaCaja) {
    throw Exception("Acción Inválida. Estado:Robot14_ConCarga Accion:tomcarCaja"); 
}

void StateR14_ConCarga::depositarCaja(Caja unaCaja) {
    controlador_->depositarCajaEnEstadoConCarga(unaCaja);
}

StateR14_ConCarga::~StateR14_ConCarga() {
}


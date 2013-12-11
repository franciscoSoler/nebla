#include <Objects/DataSM_R11_R14.h>
#include <iostream>
#include <Exceptions/Exception.h>

// Only for debugging
#include <sstream>
//#include <iostream>

#include <Logger/Logger.h>

DataSM_R11_R14::DataSM_R11_R14() {    
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        robot11EstaBloqueado_[i] = false;    
    }

    // El estado inicial del robot es estar bloqueado.  
    robot14EstaBloqueado_ = true;
    // Esto es arbitrario
    cintaConPrioridad_ = 1;
    // El Robot14 solo está trabajando sobre la Cinta13 si está en 
    // estado trabajando
    cintaDeTrabajoRobot14_ = 0;
}

bool DataSM_R11_R14::insertarCajaEnCinta(uint nroCinta, Caja caja) {
    return cinta_[nroCinta-1].insertarPaquete(caja);
}

bool DataSM_R11_R14::moverCinta(uint nroCinta) {
    return cinta_[nroCinta-1].moverCinta();
}
    
bool DataSM_R11_R14::retirarCajaDeCinta(uint nroCinta, Caja & caja) {
    return cinta_[nroCinta-1].retirarPaquete(caja); 
}

uint DataSM_R11_R14::getCantidadElementosEnCinta(uint nroCinta) const {
    return cinta_[nroCinta-1].getCantidadDePaquetes();
}
 
uint DataSM_R11_R14::getCintaConPrioridad() const {
    return cintaConPrioridad_;
}

void DataSM_R11_R14::setCintaConPrioridad(uint nroCinta) {
    if (nroCinta > AMOUNT_CINTA_13) {
        throw Exception("setCintaConPrioridad: Número de cinta inválido");
    }
    cintaConPrioridad_ = nroCinta;
}

bool DataSM_R11_R14::estaRobot11Bloqueado(uint nroCinta) const {
    if (nroCinta > AMOUNT_CINTA_13) {
        throw Exception("estaRobot11Bloqueado: Número de cinta inválido");
    }
    return robot11EstaBloqueado_[nroCinta-1];
}

void DataSM_R11_R14::setEstadoBloqueoRobot11(uint nroCinta, bool estado) {
    if (nroCinta > AMOUNT_CINTA_13) {
        throw Exception("setEstadoRobot11: Número de cinta inválido");
    }
    robot11EstaBloqueado_[nroCinta-1] = estado;
}

bool DataSM_R11_R14::estaRobot14Bloqueado() const {
    return robot14EstaBloqueado_;
}

void DataSM_R11_R14::setEstadoBloqueoRobot14(bool estado) {
    robot14EstaBloqueado_ = estado;
}

uint DataSM_R11_R14::getCintaDeTrabajoRobot14() const {
    return cintaDeTrabajoRobot14_;
}

void DataSM_R11_R14::setCintaDeTrabajoRobot14(uint nroCinta) {
    if (nroCinta > AMOUNT_CINTA_13) {
        throw Exception("setCintaDeTrabajoRobot14: Número de cinta inválido");
    }

    cintaDeTrabajoRobot14_ = nroCinta;
}
DataSM_R11_R14::~DataSM_R11_R14() {
}

std::string DataSM_R11_R14::cintaToString(uint nroCinta) {
    if (nroCinta == 0 || nroCinta > AMOUNT_CINTA_13) {
        throw Exception("imprimirCinta: Número de cinta inválido");
    }
    return cinta_[nroCinta-1].toString();
}

SerializedData DataSM_R11_R14::serializeData() {
    SerializedData data;
    std::stringstream ss;
    
    ss << this->cintaConPrioridad_ << " ";
    ss << this->cintaDeTrabajoRobot14_ << " ";
    
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        SerializedData dataCinta = this->cinta_[i].serializeData();
        ss << dataCinta.data << " ";
    }
    
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        ss << this->robot11EstaBloqueado_[i] << " ";    
    }
    ss << this->robot14EstaBloqueado_; 
    
    strcpy(data.data, ss.str().c_str());
    return data;
}

void DataSM_R11_R14::deserializeData(SerializedData data) {
    std::stringstream ss;
    ss << data.data;
    
    ss >> this->cintaConPrioridad_;
    ss >> this->cintaDeTrabajoRobot14_;
    
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        SerializedData dataCinta;
        ss >> dataCinta.data;
        this->cinta_[i].deserializeData(dataCinta);
    }
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        ss >> this->robot11EstaBloqueado_[i];
    }
    ss >> this->robot14EstaBloqueado_; 
}
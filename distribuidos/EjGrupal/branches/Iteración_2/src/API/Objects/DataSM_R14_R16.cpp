#include <Objects/DataSM_R14_R16.h>

DataSM_R14_R16::DataSM_R14_R16() {
    // El estado inicial del Robot16 es no estar trabajando.
    estaRobot16Trabajando_ = false;
    // El estado inicial del Robot14 es estar desbloqueado. Está bloqueado
    // inicialmente en la Cinta13
    robot14EstaBloqueado_ = false;
    // El estado inicial del roboy16 es desbloqueado, porque está esperando en
    // la cinta
    robot16EstaBloqueado_ = false;
}

bool DataSM_R14_R16::insertarCajaEnCinta(Caja caja) {
    return cinta_.insertarPaquete(caja);
}

bool DataSM_R14_R16::moverCinta() {
    return cinta_.moverCinta();
}
    
bool DataSM_R14_R16::retirarCajaDeCinta(Caja & caja) {
    return cinta_.retirarPaquete(caja); 
}

std::string DataSM_R14_R16::cintaToString() {
    return cinta_.toString();
}
    
uint DataSM_R14_R16::getCantidadElementosEnCinta() const {
    return cinta_.getCantidadDePaquetes();
}

bool DataSM_R14_R16::estaRobot14Bloqueado() const {
    return robot14EstaBloqueado_;
}

void DataSM_R14_R16::setEstadoBloqueoRobot14(bool estado) {
    robot14EstaBloqueado_ = estado;
}

bool DataSM_R14_R16::estaRobot16Bloqueado() const {
    return robot16EstaBloqueado_;
}

void DataSM_R14_R16::setEstadoBloqueoRobot16(bool estado) {
    robot16EstaBloqueado_ = estado;
}

bool DataSM_R14_R16::estaRobot16Trabajando() const {
    return estaRobot16Trabajando_;
}

void DataSM_R14_R16::setEstadoTrabajoRobot16(bool estado) {
    estaRobot16Trabajando_ = estado;
}

DataSM_R14_R16::~DataSM_R14_R16() {
}

SerializedData DataSM_R14_R16::serializeData() {
    SerializedData data;
    std::stringstream ss;
    
    SerializedCinta dataCinta = this->cinta_.serializeData();
    ss << dataCinta.data << " ";
    ss << this->estaRobot16Trabajando_ << " ";
    ss << this->robot14EstaBloqueado_ << " ";
    ss << this->robot16EstaBloqueado_ << " ";
    
    strcpy(data.data, ss.str().c_str());
    return data;
}

void DataSM_R14_R16::deserializeData(SerializedData data) {
    std::stringstream ss;
    ss << data.data;

    // ss >> dataCinta.data;
    this->cinta_.deserializeData( ss );
 
    ss >> this->estaRobot16Trabajando_; 
    ss >> this->robot14EstaBloqueado_; 
    ss >> this->robot16EstaBloqueado_; 
}

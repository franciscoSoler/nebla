#ifndef DATASM_R11_R14_H
#define	DATASM_R11_R14_H

#include <Objects/Cinta.h>
#include <Common.h>
#include <definedTypes.h>
#include <string>

class DataSM_R11_R14 {
    
public:
    DataSM_R11_R14();
    bool insertarCajaEnCinta(uint nroCinta, Caja caja);
    bool moverCinta(uint nroCinta);
    bool retirarCajaDeCinta(uint nroCinta, Caja & caja);
    std::string cintaToString(uint nroCinta);
    
    uint getCantidadElementosEnCinta(uint nroCinta) const;
    uint getCintaConPrioridad() const;

    void setCintaConPrioridad(uint nroCinta);
    bool estaRobot11Bloqueado(uint nroCinta) const;
    // True indica bloqueado
    void setEstadoBloqueoRobot11(uint nroCinta, bool estado);
    bool estaRobot14Bloqueado() const;
    // True indica bloqueado
    void setEstadoBloqueoRobot14(bool estado); 
    uint getCintaDeTrabajoRobot14() const;
    void setCintaDeTrabajoRobot14(uint nroCinta);
    
    virtual ~DataSM_R11_R14();
    
private:
    Cinta<Caja, CINTA_13_CAPACITY> cinta_[AMOUNT_CINTA_13];
    bool robot11EstaBloqueado_[AMOUNT_CINTA_13];
    // Variable que permite indicarle al Robot11 si el Robot14 está bloqueado
    // esperando que lo despierte el Robot11
    bool robot14EstaBloqueado_;
    // Variable que indica en qué cinta estuvo bloqueada anteriormente 
    // el Robot14 en caso de que ambos Robots estuvieran bloqueados
    uint cintaConPrioridad_;
    // Indica en quécinta estátrabajando el Robot14
    uint cintaDeTrabajoRobot14_;
};

#endif	/* DATASM_R11_R14_H */


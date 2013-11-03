#ifndef DATASM_R14_R16_H
#define	DATASM_R14_R16_H

#include <Objects/Cinta.h>
#include <Common.h>
#include <definedTypes.h>
#include <string>

class DataSM_R14_R16 {
public:
    DataSM_R14_R16();
    bool insertarCajaEnCinta(Caja caja);
    bool moverCinta();
    bool retirarCajaDeCinta(Caja & caja);
    std::string cintaToString();
    
    uint getCantidadElementosEnCinta() const;
    bool estaRobot14Bloqueado() const;
    // True indica bloqueado
    void setEstadoBloqueoRobot14(bool estado);
    bool estaRobot16Bloqueado() const;
    // True indica bloqueado
    void setEstadoBloqueoRobot16(bool estado);
    bool estaRobot16Trabajando() const;
    void setEstadoTrabajoRobot16(bool estado);
    
    virtual ~DataSM_R14_R16();
    
private:
    
private:
    Cinta<Caja, CINTA_15_CAPACITY> cinta_;
    // Variable que permite indicarle al Robot16 si el Robot11 está bloqueado
    // esperando que el Robot16 saque cajas de la cinta porque estaba llena
    bool robot14EstaBloqueado_;
    // Permite indicarle al Robot14 si el Robot16 está bloqueado porque no
    // hay cajas sobre la cinta
    bool robot16EstaBloqueado_;
    // Indica cuando el Robot16 está trabajando. Esto es cuando está moviendo
    // la cinta y/o esté moviendo una caja de la Cinta15 al despacho
    bool estaRobot16Trabajando_;
};

#endif	/* DATASM_R14_R16_H */


#ifndef STATER16_TRABAJANDO_H
#define	STATER16_TRABAJANDO_H

#include <API/Robot16/State/IStateR16.h>
#include <API/Robot16/ControllerRobot16.h>

// Forward Declaration
class ControllerRobot16;

class StateR16_Trabajando : public IStateR16 {
public:
    StateR16_Trabajando(ControllerRobot16* controlador);
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);
    virtual ~StateR16_Trabajando();

private:
    ControllerRobot16* controlador_;
};

#endif	/* STATER16_TRABAJANDO_H */


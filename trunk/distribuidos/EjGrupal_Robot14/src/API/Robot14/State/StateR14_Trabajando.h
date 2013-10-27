#ifndef STATER14_TRABAJANDO_H
#define	STATER14_TRABAJANDO_H

#include <API/Robot14/State/IStateR14.h>
#include <API/Robot14/ControllerRobot14.h>

// Forward Declaration
class ControllerRobot14;

class StateR14_Trabajando : public IStateR14 {
public:
    StateR14_Trabajando(ControllerRobot14* controlador);
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);
    virtual ~StateR14_Trabajando();

private:
    ControllerRobot14* controlador_;
};

#endif	/* STATER14_TRABAJANDO_H */


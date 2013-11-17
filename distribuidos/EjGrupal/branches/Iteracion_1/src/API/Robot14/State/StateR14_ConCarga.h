#ifndef STATER14_CONCARGA_H
#define	STATER14_CONCARGA_H

#include <API/Robot14/State/IStateR14.h>
#include <API/Robot14/ControllerRobot14.h>

// Forward Declaration
class ControllerRobot14;

class StateR14_ConCarga : public IStateR14 {
public:
    StateR14_ConCarga(ControllerRobot14* controlador);
    bool moverCinta(uint nroCinta);
    bool tomarCaja(Caja & unaCaja, uint nroCinta);
    void depositarCaja(Caja unaCaja);
    virtual ~StateR14_ConCarga();

private:
    ControllerRobot14* controlador_;
};

#endif	/* STATER14_CONCARGA_H */


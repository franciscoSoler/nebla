#ifndef STATE_R14_DURMIENDO_
#define STATE_R14_DURMIENDO_

#include <API/Robot14/State/IStateR14.h>
#include <API/Robot14/ControllerRobot14.h>

// Forward Declaration
class ControllerRobot14;

class StateR14_Durmiendo : public IStateR14 {
public:
    StateR14_Durmiendo(ControllerRobot14* controlador);
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);
    virtual ~StateR14_Durmiendo();

private:
    ControllerRobot14* controlador_;
};

#endif /* STATE_R14_DURMIENDO_ */


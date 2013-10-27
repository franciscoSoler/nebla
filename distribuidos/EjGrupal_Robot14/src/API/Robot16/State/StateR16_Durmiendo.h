#ifndef STATE_R16_DURMIENDO_
#define STATE_R16_DURMIENDO_

#include <API/Robot16/State/IStateR16.h>
#include <API/Robot16/ControllerRobot16.h>

// Forward Declaration
class ControllerRobot16;

class StateR16_Durmiendo : public IStateR16 {
public:
    StateR16_Durmiendo(ControllerRobot16* controlador);
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);
    virtual ~StateR16_Durmiendo();

private:
    ControllerRobot16* controlador_;
};

#endif /* STATE_R16_DURMIENDO_ */


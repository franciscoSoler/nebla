#ifndef I_CONTROLLER_ROBOT_14_
#define I_CONTROLLER_ROBOT_14_

#include <Common.h>

class IControllerRobot14 {
public:
    virtual bool moverCinta() = 0;
    virtual bool tomarCaja(Caja & unaCaja) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IControllerRobot14() {};
};

#endif /* I_CONTROLLER_ROBOT_14_ */


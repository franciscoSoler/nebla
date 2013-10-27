#ifndef I_CONTROLLER_ROBOT_11_
#define I_CONTROLLER_ROBOT_11_

#include <common.h>

class IControllerRobot11 {
public:
    virtual Caja tomarCaja() = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IControllerRobot11() {};
};

#endif /* I_CONTROLLER_ROBOT_11_ */


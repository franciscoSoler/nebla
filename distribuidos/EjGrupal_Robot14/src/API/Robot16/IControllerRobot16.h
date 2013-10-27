#ifndef I_CONTROLLER_ROBOT_16_
#define I_CONTROLLER_ROBOT_16_

#include <common.h>

class IControllerRobot16 {
    
public:
    virtual bool moverCinta() = 0;
    virtual bool tomarCaja(Caja & unaCaja) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IControllerRobot16() {};
};

#endif /* I_CONTROLLER_ROBOT_16_ */


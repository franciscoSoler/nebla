#ifndef I_STATE_R14_
#define I_STATE_R14_

#include <common.h>

class IStateR14 {
public:
    virtual bool moverCinta() = 0;
    virtual bool tomarCaja(Caja & unaCaja) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IStateR14() {};
};

#endif /* I_STATE_R14_ */


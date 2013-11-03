#ifndef I_STATE_R16_
#define I_STATE_R16_

#include <Common.h>

class IStateR16 {
public:
    virtual bool moverCinta() = 0;
    virtual bool tomarCaja(Caja & unaCaja) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IStateR16() {};
};

#endif /* I_STATE_R16_ */


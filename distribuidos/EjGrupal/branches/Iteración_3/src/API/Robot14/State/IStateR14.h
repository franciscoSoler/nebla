#ifndef I_STATE_R14_
#define I_STATE_R14_

#include <Common.h>
#include <definedTypes.h>

class IStateR14 {
public:
    virtual bool moverCinta(uint nroCinta) = 0;
    virtual bool tomarCaja(Caja & unaCaja, uint nroCinta) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IStateR14() {};
};

#endif /* I_STATE_R14_ */


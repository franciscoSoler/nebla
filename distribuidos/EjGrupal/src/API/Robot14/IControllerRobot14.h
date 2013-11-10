#ifndef I_CONTROLLER_ROBOT_14_
#define I_CONTROLLER_ROBOT_14_

#include <Common.h>
#include <definedTypes.h>

class IControllerRobot14 {
public:
    /* El cliente se bloquea mientras que no haya cajas en las Cintas
     */
    virtual void comenzarATrabajar() = 0;
    /* Desde el momento en el que se llama a estos dos métodos
     * la cinta se para hasta que se llame a moverCinta(uint nroCinta)
     */
    virtual bool estaRobot11BloqueadoEnCinta(uint nroCinta) = 0;
    virtual uint obtenerCantidadDeCajasEnCinta(uint nroCinta) = 0;

    /* Si la cinta estaba parada, se activa con este método
     */
    virtual bool moverCinta(uint nroCinta) = 0;
    virtual bool tomarCaja(Caja & unaCaja, uint nroCinta) = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IControllerRobot14() {};
};

#endif /* I_CONTROLLER_ROBOT_14_ */

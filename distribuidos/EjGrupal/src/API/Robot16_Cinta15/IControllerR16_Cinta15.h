#ifndef I_CONTROLLER_R16_CINTA_15_H_
#define I_CONTROLLER_R16_CINTA_15_H_

#include <Common.h>

class IControllerR16_Cinta15 {
    
public:
    virtual void recibirOrdenParaMoverCaja() = 0;
    virtual bool moverCinta() = 0;
    virtual bool tomarCajaCinta15(Caja & unaCaja) = 0;
    /* Retorna true si terminó de procesarse el producto asociado a la orden
     * de compra del producto. 
     */
    virtual bool depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT) = 0;
    virtual void informarAlDespachoProductoTerminado(long idNroOrden, 
                                                     TipoProducto tipo) = 0;
    virtual ~IControllerR16_Cinta15() {};
};

#endif /* I_CONTROLLER_R16_CINTA_15_H_ */


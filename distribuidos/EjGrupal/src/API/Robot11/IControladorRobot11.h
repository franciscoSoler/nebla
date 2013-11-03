/* 
 * File:   IControladorRobot11.h
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 12:52
 */

#ifndef ICONTROLADORROBOT11_H
#define	ICONTROLADORROBOT11_H

#include "../Common.h"

class IControladorRobot11 {
public:
    virtual void iniciarControlador(int numRobot) = 0;
    virtual bool buscarProximoGabinete(EspecifProd *piezas) = 0;
    virtual void avanzarCinta() = 0;
    virtual bool agregarPantalla(EspecifPiezaDeProd piezaDeProd) = 0;
    virtual void pedirPiezaAlAGV(TipoProducto tipoPieza) = 0;
    virtual Caja cerrarYTomarCaja() = 0;
    virtual void depositarCaja(Caja unaCaja) = 0;
    virtual ~IControladorRobot11 () {};
};

#endif	/* ICONTROLADORROBOT11_H */


/* 
 * File:   IControladorRobot12.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 13:55
 */

#ifndef ICONTROLADORROBOT12_H
#define	ICONTROLADORROBOT12_H

#include "../Common.h"

class IControladorRobot12 {
public:
    virtual void iniciarControlador(int numRobot) = 0;
    virtual EspecifProd esperarProximoGabinete() = 0;
    virtual bool agregarConector(EspecifPiezaDeProd piezaDeProd) = 0;
    virtual void pedirPiezaAlAGV(TipoProducto tipoPieza) = 0;
    virtual void finalizarEnsamble() = 0;
    virtual ~IControladorRobot12 () {};
};

#endif	/* ICONTROLADORROBOT12_H */


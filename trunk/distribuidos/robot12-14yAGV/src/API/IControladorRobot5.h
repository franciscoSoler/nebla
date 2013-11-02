/* 
 * File:   IControladorRobot5.h
 * Author: francisco
 *
 * Created on 31 de octubre de 2013, 16:11
 */

#ifndef ICONTROLADORROBOT5_H
#define	ICONTROLADORROBOT5_H

#include "../Common.h"

class IControladorRobot5 {
public:
    virtual void iniciarControlador() = 0;
    virtual void atenderPedidoAGV() = 0;
    virtual void llenarCinta() = 0;
    virtual ~IControladorRobot5 () {};
};

#endif	/* ICONTROLADORROBOT5_H */


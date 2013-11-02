/* 
 * File:   IControladorAGV.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 19:59
 */

#ifndef ICONTROLADORAGV_H
#define	ICONTROLADORAGV_H

#include "../Common.h"

class IControladorAGV {
public:
    virtual void iniciarControlador(int id_AGV) = 0;
    virtual TipoPieza atenderPedidos() = 0;
    virtual Canasto buscarPieza(TipoPieza tipoPieza) = 0;
    virtual void reponerCanasto(Canasto canasto) = 0;
    virtual ~IControladorAGV() {};
};

#endif	/* ICONTROLADORAGV_H */


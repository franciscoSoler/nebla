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
    virtual CintaTransportadora_6 esperarProximoGabinete() = 0;
    virtual BufferCanastos obtenerBufferCanastos() = 0;
    virtual void pedirPiezaFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) = 0;
    virtual void pedirCanastoFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) = 0;
    virtual void posicionarCanasto(BufferCanastos canastos) = 0;
    //virtual bool agregarConector(EspecifPiezaDeProd piezaDeProd) = 0;
    virtual void pedirPiezaAlAGV(TipoPieza tipoPieza, int posicionPieza) = 0;
    virtual void finalizarEnsamble() = 0;
    virtual ~IControladorRobot12 () {};
};

#endif	/* ICONTROLADORROBOT12_H */


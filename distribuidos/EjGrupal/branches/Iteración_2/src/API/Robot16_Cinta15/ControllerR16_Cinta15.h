#ifndef CONTROLLER_R16_CINTA_15_H
#define	CONTROLLER_R16_CINTA_15_H

#include <API/Robot16_Cinta15/IControllerR16_Cinta15.h>
#include <Comunicaciones/Objects/CommSemaphoreMutex.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <API/Objects/DataSM_R14_R16.h>
#include <definedTypes.h>
#include <Common.h>

#include <Comunicaciones/Objects/CommMsgHandler.h>

// IPCs de Fede
#include <API/Objects/SmMemAlmacenProductosTerminados.h>

class ControllerR16_Cinta15 : public IControllerR16_Cinta15 {
    
public:
    ControllerR16_Cinta15();
    void recibirOrdenParaMoverCaja();
    bool moverCinta();
    bool tomarCajaCinta15(Caja & unaCaja);
    bool depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT);
    void informarAlDespachoProductoTerminado(long idnroOrdenAPT, TipoProducto tipo);
    virtual ~ControllerR16_Cinta15();
    
private:
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    void obtenerMutexSincronismo();
    void liberarMutexSincronismo();
    bool avanzarCinta();
    
private:
    char buffer_[255];
    
    DataSM_R14_R16* shMem_R14_R16_Data_;
    SmMemAlmacenProductosTerminados shMem_APT_;
    
    IPC::SharedMemory<DataSM_R14_R16> shMem_R14_R16_;
    IPC::CommSemaphoreMutex semMutex_shMem_R14_R16_;
    IPC::CommSemaphoreMutex semMutex_shMem_APT_;
    IPC::Semaphore semMutex_sincronismo_R16_;
    IPC::CommSemaphore semR14_Cinta15_;

    CommMsgHandler inputQueueR16_Cinta15_;
    CommMsgHandler inputQueueDespacho_;
    Msg_AvisoCajaEnCinta15 mensaje_;
};

#endif	/* CONTROLLERROBOT16_H */

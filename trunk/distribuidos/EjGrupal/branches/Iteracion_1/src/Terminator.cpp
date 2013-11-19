/* 
 * File:   Terminator.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>

#include "Exceptions/Exception.h"
#include "Logger/Logger.h"

#include "IPCs/Semaphore/Semaphore.h"

#include "IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

#include "IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"

#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EspacioAlmacenPiezasSharedMemory.h"

#include "API/Objects/DataSM_R11_R14.h"
#include "API/Objects/DataSM_R14_R16.h"
#include "IPCs/IPCTemplate/SharedMemory.h"

#include "API/Objects/DataSM_R14_R16.h"
#include "IPCs/IPCTemplate/SharedMemory.h"

#include "IPCs/Barrios/Cola.h"
#include "IPCs/Barrios/MemoriaCompartida.h"
#include "MsgQueue.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator:");

        // Robot 5
        IPC::Semaphore semaforoApiRobot5("Api robot 5");
        semaforoApiRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_API_ROBOT_5, 1);
        semaforoApiRobot5.destroy();
    
        // Robot 5 - Cinta
        IPC::PedidosProduccionMessageQueue colaPedidos("PedidosProduccionMessageQueue");
        colaPedidos.getMessageQueue(DIRECTORY_ROBOT_5, ID_COLA_PEDIDOS_PRODUCCION);
        colaPedidos.destroy();

        IPC::Semaphore semaforoAccesoEstadoRobot5("Estado Robot 5");
        semaforoAccesoEstadoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
        semaforoAccesoEstadoRobot5.destroy();
        IPC::EstadoRobot5SharedMemory estadoRobot5ShMem("EstadoRobot5SharedMemory");
        estadoRobot5ShMem.getSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);
        estadoRobot5ShMem.destroy();

        IPC::Semaphore semaforoBloqueoRobot5("Bloqueo Robot 5");
        semaforoBloqueoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
        semaforoBloqueoRobot5.destroy();

        IPC::Semaphore semaforoBloqueoRobot11("Bloqueo Robot 11");
        IPC::Semaphore semBloqueoRobot12("semBloqueoRobot12");
        semaforoBloqueoRobot11.getSemaphore(DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, CANTIDAD_CINTAS_6);
        semBloqueoRobot12.getSemaphore(DIRECTORY_ROBOT_12, ID_SEM_BLOQUEO_ROBOT_12, CANTIDAD_CINTAS_6);
        semaforoBloqueoRobot11.destroy();
        semBloqueoRobot12.destroy();

        IPC::Semaphore semaforoAccesoCinta6("Acceso Cinta 6");
        semaforoAccesoCinta6.getSemaphore(DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, CANTIDAD_CINTAS_6);
        semaforoAccesoCinta6.destroy();

        IPC::Cinta6SharedMemory cinta6_0("Cinta 6 Share dMemory 0");
        cinta6_0.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
        cinta6_0.destroy();
        IPC::Cinta6SharedMemory cinta6_1("Cinta 6 Share dMemory 1");
        cinta6_1.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_1);
        cinta6_1.destroy();

        //Robot 5 - AGV
        IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1("BufferCanastoEntre5yAGVSharedMemory 0");
        IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2("BufferCanastoEntre5yAGVSharedMemory 1");
        IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3("BufferCanastoEntre5yAGVSharedMemory 2");
        shMemPasajeCanastoEntre5yAGV1.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
        shMemPasajeCanastoEntre5yAGV2.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
        shMemPasajeCanastoEntre5yAGV3.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
        shMemPasajeCanastoEntre5yAGV1.destroy();
        shMemPasajeCanastoEntre5yAGV2.destroy();
        shMemPasajeCanastoEntre5yAGV3.destroy();

        // Do the same with the semaphores
        IPC::Semaphore semBloqueoAGV("semBloqueoAGV");
        IPC::Semaphore semBufferAGV_5("semBufferAGV_5");
        IPC::Semaphore semBufferCanastos("semMemCanastos");
        semBloqueoAGV.getSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, CANTIDAD_AGVS);
        semBloqueoAGV.destroy();
        semBufferAGV_5.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, CANTIDAD_AGVS);
        semBufferAGV_5.destroy();
        semBufferCanastos.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, CANTIDAD_AGVS);
        semBufferCanastos.destroy();

        // Do the same with the queues
        IPC::PedidosAgvMessageQueue colaPedidosAGV_5;
        colaPedidosAGV_5.getMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);
        colaPedidosAGV_5.destroy();
        
        // Robots cintas - AGV
        
        IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
        colaPedidosCanastos.getMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);
        colaPedidosCanastos.destroy();

        IPC::BufferCanastosSharedMemory shMemBufferCanastos0 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos0");
        IPC::BufferCanastosSharedMemory shMemBufferCanastos1 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos1");
        IPC::BufferCanastosSharedMemory shMemBufferCanastos2 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos2");
        shMemBufferCanastos0.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
        shMemBufferCanastos1.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);
        shMemBufferCanastos2.getSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
        shMemBufferCanastos0.destroy();
        shMemBufferCanastos1.destroy();
        shMemBufferCanastos2.destroy();
        
        //Robots 11 y 12
        IPC::Barrera1112MessageQueue cola11_A_121;
        IPC::Barrera1112MessageQueue cola11_A_122;
        cola11_A_121.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_1);
        cola11_A_122.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_2);
        cola11_A_121.destroy();
        cola11_A_122.destroy();

        IPC::Barrera1112MessageQueue cola12_A_111;
        IPC::Barrera1112MessageQueue cola12_A_112;
        cola12_A_111.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_1);
        cola12_A_112.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_2);
        cola12_A_111.destroy();
        cola12_A_112.destroy();
        

        // IPCs Torres

        IPC::SharedMemory<DataSM_R11_R14> SM_R11_R14("SM_R11_R14");
        SM_R11_R14.getSharedMemory(DIRECTORY_ROBOT_11, SM_R11_R14_ID);
        SM_R11_R14.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_R11_R14 destruido");

        IPC::SharedMemory<DataSM_R14_R16> SM_R14_R16("SM_R14_R16");
        SM_R14_R16.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        SM_R14_R16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_R14_R16 destruido");

        IPC::Semaphore semMutexSM_R11_R14("semMutexSM_R11_R14");
        semMutexSM_R11_R14.getSemaphore(DIRECTORY_ROBOT_11, SEM_MUTEX_SM_R11_R14_ID, 1);
        semMutexSM_R11_R14.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_R11_R14 destruido");

        IPC::Semaphore semMutexSM_R14_R16("semMutexSM_R14_R16");
        semMutexSM_R14_R16.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
        semMutexSM_R14_R16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_R14_R16 destruido");

        IPC::Semaphore semMutex_sincronismo_R16("semMutex_sincronismo_R16");
        semMutex_sincronismo_R16.getSemaphore(DIRECTORY_ROBOT_16, SEM_MUTEX_SINCRONISMO_R16_ID, 1);
        semMutex_sincronismo_R16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_sincronismo_R16 destruido");

        IPC::Semaphore semR11_Cinta13("semR11_Cinta13");
        semR11_Cinta13.getSemaphore(DIRECTORY_ROBOT_11, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
        semR11_Cinta13.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR11_Cinta13 destruido");

        IPC::Semaphore semR14_Cinta13("semR14_Cinta13");
        semR14_Cinta13.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA13_ID, 1);
        semR14_Cinta13.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR14_Cinta13 destruido");

        IPC::Semaphore semR14_Cinta15("semR14_Cinta15");
        semR14_Cinta15.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA15_ID, 1);
        semR14_Cinta15.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR14_Cinta15 destruido");

        /*** Queues ***/
        IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
        inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
        inputQueueDespacho.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueDespacho destruido");

        IPC::MsgQueue inputQueueR16_Cinta15("inputQueueR16_Cinta15");
        inputQueueR16_Cinta15.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CINTA15_INPUT_ID);
        inputQueueR16_Cinta15.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueR16_Cinta15 destruido");

        IPC::MsgQueue inputQueueR16_Despacho("inputQueueR16_Despacho");
        inputQueueR16_Despacho.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
        inputQueueR16_Despacho.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueR16_Despacho Destruido");

        IPC::MsgQueue inputQueueCliente("inputQueueCliente");
        inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
        inputQueueCliente.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueCliente destruido");

        IPC::MsgQueue R16_Cliente_Queue("R16_Cliente_Queue");
        R16_Cliente_Queue.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
        R16_Cliente_Queue.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC R16_Cliente_Queue destruido");

        // Fin IPCs Torres

    
        IPC::VendedorLibreMessageQueue vendedores("Vendedores Msg Queue");
        vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
        vendedores.destroy();
        IPC::ClientesMessageQueue clientes("Clientes Msg Queue");
        clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
        clientes.destroy();
        IPC::PedidosVendedorMessageQueue pedidos("Pedidos Msg Queue");
        pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
        pedidos.destroy();

	
        IPC::Semaphore mutexAlmacenDePiezas("mutexAlmacenDePiezas");
        mutexAlmacenDePiezas.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ALMACEN_PIEZAS, 1);
        mutexAlmacenDePiezas.destroy();

        IPC::EspacioAlmacenPiezasSharedMemory shMemAlmacenDePiezas = IPC::EspacioAlmacenPiezasSharedMemory("shMemAlmacenDePiezas");
        shMemAlmacenDePiezas.getSharedMemory(DIRECTORY_APIEZAS, LETRA_SHMEM_ALMACEN_PIEZAS);
        shMemAlmacenDePiezas.destroy();

        IPC::Semaphore esperaRepositorCanasto("Espera Repositor Canasto");
        esperaRepositorCanasto.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_CANASTOS, 1);
        esperaRepositorCanasto.destroy();

        IPC::Semaphore esperaRepositorGabinete("Espera Repositor Gabinete");
        esperaRepositorGabinete.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_GABINETES, 1);
        esperaRepositorGabinete.destroy();
    }
    
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }

    Cola<consulta_almacen_piezas_t> consultasAlmacen(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    consultasAlmacen.destruir();
    Cola<respuesta_almacen_piezas_t> respuestasAlmacen(DIRECTORY_VENDEDOR, ID_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    respuestasAlmacen.destruir();
   
    MemoriaCompartida shmemAlmacenTerminados(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    shmemAlmacenTerminados.liberar();
    MemoriaCompartida shmemNumeroOrdenCompra(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, sizeof(int));
    shmemNumeroOrdenCompra.liberar();

    IPC::Semaphore mutexAlmacenTerminados("Mutex Almacen Terminados");
    mutexAlmacenTerminados.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    mutexAlmacenTerminados.destroy();
    
    IPC::Semaphore mutexOrdenDeCompra("mutexOrdenDeCompra");
    mutexOrdenDeCompra.getSemaphore(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, 1);
    mutexOrdenDeCompra.destroy();
}


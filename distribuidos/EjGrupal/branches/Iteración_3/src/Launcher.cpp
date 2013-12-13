/* 
 * File:   Launcher.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>

#include <sys/stat.h>
#include <sys/types.h>

#include "Common.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"

#include "IPCs/Semaphore/Semaphore.h"

#include "IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EspacioAlmacenPiezasSharedMemory.h"

#include "API/Objects/DataSM_R11_R14.h"
#include "API/Objects/DataSM_R14_R16.h"
#include <API/Objects/Util.h>

#include "IPCs/IPCTemplate/SharedMemory.h"
#include "IPCs/IPCTemplate/MsgQueue.h"

//#include "IPCs/Barrios/MemoriaCompartida.h"
#include "VendedorLibreMessageQueue.h"
#include "ClientesMessageQueue.h"
#include "PedidosVendedorMessageQueue.h"
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ServersManager.h>


// static char buffer[255];
// static char param1[20];
// static char param2[20];

void createIPCs();
void createDirectory(std::string directoryName);

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();
        
        createDirectory(DIRECTORY_AGV);
        createDirectory(DIRECTORY_ROBOT_5);
        createDirectory(DIRECTORY_ROBOT_11);
        createDirectory(DIRECTORY_ROBOT_12);
        createDirectory(DIRECTORY_ROBOT_14);
        createDirectory(DIRECTORY_ROBOT_16);
        createDirectory(DIRECTORY_VENDEDOR);
        createDirectory(DIRECTORY_CLIENTE);
        createDirectory(DIRECTORY_DESPACHO);
        createDirectory(DIRECTORY_APT);
        createDirectory(DIRECTORY_APIEZAS);
                
        createIPCs();
        
        Util::createProcess("Robot5-Agv", 1);
        Util::createProcess("Robot5-Cinta", 1);
        Util::createProcess("AlmacenPiezas", 1);
        Util::createProcess("Robot11", 2);
        Util::createProcess("Robot12", 2);
        Util::createProcess("Robot14");
        Util::createProcess("Robot16_Cinta15");
        Util::createProcess("Robot16_Despacho");
        Util::createProcess("Despacho");
        Util::createProcess("AGV", 3);
        Util::createProcess("Vendedor", 5, 1);

        
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}

void createDirectory(std::string directoryName) {
    mkdir(directoryName.c_str(), 0777);
}

void createIPCs() {
    //Logger::getInstance().createLog();
    Logger::getInstance().setProcessInformation("Launcher:");
    
    // Robot 5
    IPC::Semaphore semaforoApiRobot5("Api robot 5");
    semaforoApiRobot5.createSemaphore(DIRECTORY_ROBOT_5, ID_SEM_API_ROBOT_5, 1);
    semaforoApiRobot5.initializeSemaphore(0,1);

    // Robot 5 - Cinta
    IPC::PedidosProduccionMessageQueue colaPedidos("PedidosProduccionMessageQueue");
    colaPedidos.createMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_PRODUCCION);
            
    IPC::Semaphore semaforoAccesoEstadoRobot5("Estado Robot 5");
    semaforoAccesoEstadoRobot5.createSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
    semaforoAccesoEstadoRobot5.initializeSemaphore(0,1);
    IPC::EstadoRobot5SharedMemory estadoRobot5ShMem("EstadoRobot5SharedMemory");
    estadoRobot5ShMem.createSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);
    semaforoAccesoEstadoRobot5.wait();
    {
        EstadoRobot5 estadoRobot5;
        estadoRobot5.robot5Bloqueado = false;
        estadoRobot5ShMem.writeInfo(&estadoRobot5);
    }
    semaforoAccesoEstadoRobot5.signal();
    
    IPC::Semaphore semaforoBloqueoRobot5("Bloqueo Robot 5");
    semaforoBloqueoRobot5.createSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
    semaforoBloqueoRobot5.initializeSemaphore(0,0);
    
    IPC::Semaphore semaforoBloqueoRobot11("Bloqueo Robot 11");
    IPC::Semaphore semBloqueoRobot12("semBloqueoRobot12");
    semaforoBloqueoRobot11.createSemaphore(DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, CANTIDAD_CINTAS_6);
    semBloqueoRobot12.createSemaphore(DIRECTORY_ROBOT_12, ID_SEM_BLOQUEO_ROBOT_12, CANTIDAD_CINTAS_6);
    for (int i = 0; i < CANTIDAD_CINTAS_6; ++i) {
        semaforoBloqueoRobot11.initializeSemaphore(i, 0);
        semBloqueoRobot12.initializeSemaphore(i, 0);
    }
    
    IPC::Semaphore semaforoAccesoCinta6("Acceso Cinta 6");
    semaforoAccesoCinta6.createSemaphore(DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, CANTIDAD_CINTAS_6);
        for (int i = 0; i < CANTIDAD_CINTAS_6; ++i) {
        semaforoAccesoCinta6.initializeSemaphore(i, 1);
    }
    
    IPC::Cinta6SharedMemory cinta6_0("Cinta 6 Share dMemory 0");
    cinta6_0.createSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
    IPC::Cinta6SharedMemory cinta6_1("Cinta 6 Share dMemory 1");
    cinta6_1.createSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_1);

    CintaTransportadora_6 cinta6;
    cinta6.cantLibres = BUFF_SIZE_CINTA_6;
    cinta6.puntoLectura = 0;
    cinta6.robot11Durmiendo = false;
    for (int i = 0; i < BUFF_SIZE_CINTA_6; i++) cinta6.lugarVacio[i] = true;

    semaforoAccesoCinta6.wait(0);
    {
        cinta6_0.writeInfo(&cinta6);
    }
    semaforoAccesoCinta6.signal(0);    
    
    semaforoAccesoCinta6.wait(1);
    {
        cinta6_1.writeInfo(&cinta6);
    }
    semaforoAccesoCinta6.signal(1);

    //Robot 5 - AGV
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1("BufferCanastoEntre5yAGVSharedMemory 0");
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2("BufferCanastoEntre5yAGVSharedMemory 1");
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3("BufferCanastoEntre5yAGVSharedMemory 2");
    shMemPasajeCanastoEntre5yAGV1.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
    shMemPasajeCanastoEntre5yAGV2.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
    shMemPasajeCanastoEntre5yAGV3.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
    
    // Do the same with the semaphores
    IPC::Semaphore semBloqueoAGV("semBloqueoAGV");
    IPC::Semaphore semBufferAGV_5("semBufferAGV_5");
    IPC::Semaphore semBufferCanastos("semMemCanastos");
    semBloqueoAGV.createSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, CANTIDAD_AGVS);
    semBufferAGV_5.createSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, CANTIDAD_AGVS);
    semBufferCanastos.createSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, CANTIDAD_AGVS);
    
    for (int i = 0; i < CANTIDAD_AGVS; i++) {
        semBloqueoAGV.initializeSemaphore(i, 0);
        semBufferAGV_5.initializeSemaphore(i, 1);
        semBufferCanastos.initializeSemaphore(i, 1);
    }
    
    // Do the same with the queues
    IPC::PedidosAgvMessageQueue colaPedidosAGV_5;
    colaPedidosAGV_5.createMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);
        
    // Robots cintas - AGV
    BufferCanastos canastos;
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
        canastos.canastos[i].cantidadPiezas = 0;
        canastos.canastos[i].tipoPieza = NULL_PIEZA;
        canastos.canastos[5].tipoPieza = PIEZA_2;
        canastos.robotCinta1EsperaPorElLugarNumero = -1;
        canastos.robotCinta2EsperaPorElLugarNumero = -1;
    }
    
    IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
    colaPedidosCanastos.createMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);
    
    IPC::BufferCanastosSharedMemory shMemBufferCanastos0 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos0");
    IPC::BufferCanastosSharedMemory shMemBufferCanastos1 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos1");
    IPC::BufferCanastosSharedMemory shMemBufferCanastos2 = IPC::BufferCanastosSharedMemory("shMemBufferCanastos2");
    shMemBufferCanastos0.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
    shMemBufferCanastos1.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);
    shMemBufferCanastos2.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
    
    semBufferCanastos.wait(0);
    {
        shMemBufferCanastos0.writeInfo(&canastos);
    }
    semBufferCanastos.signal(0);
    
    semBufferCanastos.wait(1);
    {
        shMemBufferCanastos1.writeInfo(&canastos);
    }
    semBufferCanastos.signal(1);
    
    semBufferCanastos.wait(2);
    {
        shMemBufferCanastos2.writeInfo(&canastos);
    }
    semBufferCanastos.signal(2);
    
    //Robots 11 y 12
    IPC::Barrera1112MessageQueue cola11_A_12;
    cola11_A_12.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12);
    
    IPC::Barrera1112MessageQueue cola12_A_11;
    cola12_A_11.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11);
    
    /* IPCs Torres: R11-R14-R16-Despacho-Cliente-Vendedores */
    // DataSM_R11_R14 dataSM_R11_R14;
    // DataSM_R14_R16 dataSM_R14_R16;

    // Create and initialize ShMem
    IPC::SharedMemory<SerializedData> SM_R11_R14("SM_R11_R14");
    SM_R11_R14.createSharedMemory(DIRECTORY_ROBOT_11, SM_R11_R14_ID);
    // SM_R11_R14.write( & dataSM_R11_R14 );
    Logger::logMessage(Logger::IMPORTANT, "IPC SM_R11_R14 creado");

    IPC::SharedMemory<SerializedData> SM_R14_R16("SM_R14_R16");
    SM_R14_R16.createSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
    // SM_R14_R16.write( & dataSM_R14_R16 );
    Logger::logMessage(Logger::IMPORTANT, "IPC SM_R14_R16 creado");

    // Do the same with the semaphores
    IPC::Semaphore semMutexSM_R11_R14("semMutexSM_R11_R14");
    semMutexSM_R11_R14.createSemaphore(DIRECTORY_ROBOT_11, SEM_MUTEX_SM_R11_R14_ID, 1);
    semMutexSM_R11_R14.initializeSemaphore(0, 1);
    Logger::logMessage(Logger::IMPORTANT, "IPC semMutexSM_R11_R14 creado");

    IPC::Semaphore semMutexSM_R14_R16("semMutexSM_R14_R16");
    semMutexSM_R14_R16.createSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
    semMutexSM_R14_R16.initializeSemaphore(0, 1);
    Logger::logMessage(Logger::IMPORTANT, "IPC semMutexSM_R14_R16 creado");

    IPC::Semaphore semMutex_sincronismo_R16_("semMutex_sincronismo_R16");
    semMutex_sincronismo_R16_.createSemaphore(DIRECTORY_ROBOT_16, SEM_MUTEX_SINCRONISMO_R16_ID, 1);
    semMutex_sincronismo_R16_.initializeSemaphore(0, 1);
    Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_sincronismo_R16");

    IPC::Semaphore semR11_Cinta13("semR11_Cinta13");
    semR11_Cinta13.createSemaphore(DIRECTORY_ROBOT_11, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        semR11_Cinta13.initializeSemaphore(i, 0);
    }
    Logger::logMessage(Logger::IMPORTANT, "IPC semR11_Cinta13 creado");

    IPC::Semaphore semR14_Cinta13("semR14_Cinta13");
    semR14_Cinta13.createSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA13_ID, 1);
    semR14_Cinta13.initializeSemaphore(0, 0);
    Logger::logMessage(Logger::IMPORTANT, "IPC semR14_Cinta13 creado");

    IPC::Semaphore semR14_Cinta15("semR14_Cinta15");
    semR14_Cinta15.createSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA15_ID, 1);
    semR14_Cinta15.initializeSemaphore(0, 0);
    Logger::logMessage(Logger::IMPORTANT, "IPC semR14_Cinta15 creado");

    /* NOTA: La notación para colas es que el input o output es respecto
     * del nombre del proceso asociado a la cola. inputQueueClient por
     * ejemplo indica que es la cola donde el cliente recibe mensajes
     */
    IPC::MsgQueue inputQueueDespacho("inputQueueDespacho");
    inputQueueDespacho.create(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueDespacho creado");

    IPC::MsgQueue inputQueueR16_Cinta15("inputQueueR16_Cinta15");
    inputQueueR16_Cinta15.create(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CINTA15_INPUT_ID);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueR16_Cinta15 creado");

    IPC::MsgQueue inputQueueR16_Despacho("inputQueueR16_Despacho");
    inputQueueR16_Despacho.create(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueR16_Despacho creado");

    IPC::MsgQueue inputQueueCliente("inputQueueCliente");
    inputQueueCliente.create(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
    Logger::logMessage(Logger::IMPORTANT, "IPC inputQueueCliente creado");

    IPC::MsgQueue R16_Cliente_Queue("R16_Cliente_Queue");
    R16_Cliente_Queue.create(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
    Logger::logMessage(Logger::IMPORTANT, "IPC R16_Cliente_Queue creado");

    /* Fin IPCs Torres */

    IPC::VendedorLibreMessageQueue vendedores("Vendedores Msg Queue");
    vendedores.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    IPC::ClientesMessageQueue clientes("Clientes Msg Queue");
    clientes.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    IPC::PedidosVendedorMessageQueue pedidos("Pedidos Msg Queue");
    pedidos.createMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);

    IPC::MsgQueue consultasAlmacen("ConsultasAlmacen");
    consultasAlmacen.create(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    
    IPC::MsgQueue respuestasAlmacen("RespuestasAlmacen");
    respuestasAlmacen.create(DIRECTORY_VENDEDOR, ID_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    
    IPC::SharedMemory<AlmacenProductosTerminados> shmemAlmacenTerminados("shMemAlmacenTerminados");
    shmemAlmacenTerminados.createSharedMemory(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);
    
    IPC::SharedMemory<int> shmemNumeroOrdenCompra("shmemNumeroOrdenCompra");
    shmemNumeroOrdenCompra.createSharedMemory(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC);
    
    IPC::Semaphore mutexAlmacenTerminados("Acceso Almacen Terminados");
    mutexAlmacenTerminados.createSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    mutexAlmacenTerminados.initializeSemaphore(0,1);
    
    IPC::Semaphore mutexOrdenDeCompra("mutexOrdenDeCompra");
    mutexOrdenDeCompra.createSemaphore(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, 1);
    mutexOrdenDeCompra.initializeSemaphore(0, 1);
        
    /* Setea la orden de compra en uno por ser la primera. */
    mutexOrdenDeCompra.wait();
    {
	int shMemData = 1;
        shmemNumeroOrdenCompra.write(&shMemData);
    }
    mutexOrdenDeCompra.signal();
        
    /* almacén de piezas */
    IPC::Semaphore mutexAlmacenDePiezas("mutexAlmacenDePiezas");
    mutexAlmacenDePiezas.createSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ALMACEN_PIEZAS, 1);
    mutexAlmacenDePiezas.initializeSemaphore(0, 1);
    
    IPC::Semaphore esperaRepositorCanasto("Espera Repositor Canasto");
    esperaRepositorCanasto.createSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_CANASTOS, 1);
    esperaRepositorCanasto.initializeSemaphore(0, 0);
    
    IPC::Semaphore esperaRepositorGabinete("Espera Repositor Gabinete");
    esperaRepositorGabinete.createSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_GABINETES, 1);
    esperaRepositorGabinete.initializeSemaphore(0, 0);
    
    
    IPC::EspacioAlmacenPiezasSharedMemory shMemAlmacenDePiezas = IPC::EspacioAlmacenPiezasSharedMemory("shMemAlmacenDePiezas");
    shMemAlmacenDePiezas.createSharedMemory(DIRECTORY_APIEZAS, LETRA_SHMEM_ALMACEN_PIEZAS);
    
    EstructuraAlmacenPiezas estructuraAlmacen;
    for (int i = 0; i < CANTIDAD_TIPOS_PIEZAS; ++i) estructuraAlmacen.cantCanastos[i] = 1;
    for (int i = 0; i < CANTIDAD_TIPOS_GABINETES; ++i) estructuraAlmacen.cantGabinetes[i] = 0;
    
    shMemAlmacenDePiezas.writeInfo(&estructuraAlmacen);
    
}

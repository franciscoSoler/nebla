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
#include "IPCs/IPCAbstractos/MessageQueue/ComunicacionRobot5MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "API/Objects/DataSM_R11_R14.h"
#include "API/Objects/DataSM_R14_R16.h"
#include "IPCs/IPCTemplate/SharedMemory.h"
#include "IPCs/IPCTemplate/MsgQueue.h"

#include "IPCs/Barrios/Cola.h"
#include "IPCs/Barrios/MemoriaCompartida.h"

static char buffer[255];
static char param1[20];
static char param2[20];

void createIPCs();
void createDirectory(std::string directoryName);
void createProcess(std::string processName, int amountOfProcesses = 1);

int main(int argc, char* argv[]) {
    try {

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
                
        createIPCs();
        
        createProcess("Robot5", 1);
        createProcess("ControladorRobot5Agv", 1);
        createProcess("ControladorRobot5Cinta", 1);
                                
        createProcess("Robot11", 2);
        createProcess("Robot12", 2);
        createProcess("AGV", 3);
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
    Logger::getInstance().createLog();
    Logger::getInstance().setProcessInformation("Launcher:");
    
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
    
    //exclusivo Robot 5
    /* Creo la cola de comunicacion con el robot 5 con los dos procesos del controlador */
    IPC::ComunicacionRobot5MessageQueue colaComunicacionRobot5 = IPC::ComunicacionRobot5MessageQueue("colaComunicacionRobot5");
    colaComunicacionRobot5.createMessageQueue(DIRECTORY_ROBOT_5, ID_COLA_API_ROBOT_5);
    
    // Robots cintas - AGV
    BufferCanastos canastos;
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
        canastos.canastos[i].cantidadPiezas = 0;
        canastos.canastos[i].tipoPieza = PIEZA_2;
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
    IPC::Barrera1112MessageQueue cola11_A_121;
    IPC::Barrera1112MessageQueue cola11_A_122;
    cola11_A_121.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_1);
    cola11_A_122.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_2);
    
    IPC::Barrera1112MessageQueue cola12_A_111;
    IPC::Barrera1112MessageQueue cola12_A_112;
    cola12_A_111.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_1);
    cola12_A_112.createMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_2);
    
    
    // de robots 11, 14 y 16
    DataSM_R11_R14 dataSM_R11_R14;
    DataSM_R14_R16 dataSM_R14_R16;
    
	// Create and initialize ShMem
	IPC::SharedMemory<DataSM_R11_R14> SM_R11_R14("SM_R11_R14");
	SM_R11_R14.createSharedMemory(DIRECTORY_ROBOT_11, SM_R11_R14_ID);
    SM_R11_R14.write( & dataSM_R11_R14 );
    
    IPC::SharedMemory<DataSM_R14_R16> SM_R14_R16("SM_R14_R16");
	SM_R14_R16.createSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
    SM_R14_R16.write( & dataSM_R14_R16 );
   
	// Do the same with the semaphores
	IPC::Semaphore semMutexSM_R11_R14("semMutexSM_R11_R14");
	semMutexSM_R11_R14.createSemaphore(DIRECTORY_ROBOT_11, SEM_MUTEX_SM_R11_R14_ID, 1);
	semMutexSM_R11_R14.initializeSemaphore(0, 1);
    
	IPC::Semaphore semMutexSM_R14_R16("semMutexSM_R14_R16");
	semMutexSM_R14_R16.createSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
	semMutexSM_R14_R16.initializeSemaphore(0, 1);
    
    IPC::Semaphore semR11_Cinta13("semR11_Cinta13");
	semR11_Cinta13.createSemaphore(DIRECTORY_ROBOT_11, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        semR11_Cinta13.initializeSemaphore(i, 0);    
    }
	
    // TODO: Preguntar si con sólo este semáforo alcanza, o tengo que implementar
    // uno para condición de bloqueo
    IPC::Semaphore semR14("semR14");
	semR14.createSemaphore(DIRECTORY_ROBOT_14, SEM_R14_ID, 1);
	semR14.initializeSemaphore(0, 0);
    
    IPC::Semaphore semR16("semR16");
	semR16.createSemaphore(DIRECTORY_ROBOT_16, SEM_R16_ID, 1);
	semR16.initializeSemaphore(0, 0);
        
    Cola<mensaje_inicial_t> vendedores(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    vendedores.crear();
    Cola<mensaje_inicial_t> clientes(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    clientes.crear();
	
    Cola<pedido_t> pedidos(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
    pedidos.crear();
    
    Cola<consulta_almacen_piezas_t> consultasAlmacen(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    consultasAlmacen.crear();
    Cola<respuesta_almacen_piezas_t> respuestasAlmacen(DIRECTORY_VENDEDOR, ID_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    respuestasAlmacen.crear();  
    
    MemoriaCompartida shmemAlmacenTerminados(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    shmemAlmacenTerminados.crear();
    MemoriaCompartida shmemNumeroOrdenCompra(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, sizeof(int));
    shmemNumeroOrdenCompra.crear();
    
    IPC::Semaphore mutexAlmacenTerminados("Acceso Almacen Terminados");
    mutexAlmacenTerminados.createSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    
    IPC::MsgQueue outputQueueDespacho("outputQueueDespacho");
    outputQueueDespacho.create(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_OUTPUT_ID);
    
    IPC::MsgQueue inputQueueRobot16("inputQueueRobot16");
    inputQueueRobot16.create(DIRECTORY_ROBOT_16, MSGQUEUE_ROBOT16_INPUT_ID);
    
    IPC::MsgQueue outputQueueRobot16("outputQueueRobot16");
    outputQueueRobot16.create(DIRECTORY_ROBOT_16, MSGQUEUE_ROBOT16_OUTPUT_ID);
    
    IPC::MsgQueue inputQueueCliente("inputQueueCliente");
    inputQueueCliente.create(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
    
    IPC::MsgQueue outputQueueCliente("outputQueueRobot16");
    outputQueueRobot16.create(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_OUTPUT_ID);
    
    IPC::MsgQueue R16_Cliente_Queue("R16_Cliente_Queue");
    R16_Cliente_Queue.create(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
    
    IPC::MsgQueue inputQueueVendedor("inputQueueVendedor");
    inputQueueVendedor.create(DIRECTORY_VENDEDOR, MSGQUEUE_VENDOR_INPUT_ID);
    
    IPC::MsgQueue outputQueueVendedor("outputQueueVendedor");
    outputQueueVendedor.create(DIRECTORY_VENDEDOR, MSGQUEUE_VENDOR_OUTPUT_ID);  
    
    //IPCs de Fede
    IPC::Semaphore semMutex_APT("semMutex_APT");
    semMutex_APT.createSemaphore(DIRECTORY_APT, SEM_MUTEX_SM_APT_ID, 1);
    semMutex_APT.initializeSemaphore(0, 1);
    
    MemoriaCompartida shMem_APT(DIRECTORY_APT, LETRA_SHMEM_ALMACEN_TERMINADOS, 
            TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    shMem_APT.crear();
}

void createProcess(std::string processName, int amountOfProcesses) {
	pid_t pid;

    for (int i = 0; i < amountOfProcesses; i++) {
        if ((pid = fork()) < 0) {
            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
        }
        else if (pid == 0) {
            // Child process. Pass the arguments to the process and call exec
            sprintf(param1, "%d", i);
            sprintf(buffer, "./%s", processName.c_str());
            execlp(buffer, processName.c_str(), param1, (char *) 0);

            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);

            return;
        }
    }
}


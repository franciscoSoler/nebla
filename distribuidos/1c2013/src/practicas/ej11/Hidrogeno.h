#include "IPC/Semaphore.h"
#include "IPC/SharedMemoryCantidadHidrogenos.h"
#include "IPC/SharedMemoryH1Activo.h"
#include "IPC/SharedMemoryCantidadOxigenos.h"
#include "Logger.h"

class Hidrogeno {
    int nro;

    Semaphore semComienzoHidrogeno;
    Semaphore semComienzoOxigeno;
    Semaphore semFinH1;
    Semaphore semFinH2;
    Semaphore semH1Activo;
    Semaphore semH1Generado;

    Semaphore semCantHidrogenos;
    Semaphore semCantOxigenos;
    SharedMemoryH1Activo smH1Activo;
    SharedMemoryCantidadHidrogenos smCantHidrogenos;
    SharedMemoryCantidadOxigenos smCantOxigenos;
    
    Logger logger;

    bool soyH1_;
            
    void salir();
    bool soyH1();
    bool hayAtomos();
public:
    Hidrogeno(int nro);
    void iniciar();
    void terminar();

};

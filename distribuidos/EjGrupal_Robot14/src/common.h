#ifndef COMMON_H_
#define COMMON_H_

#define DIRECTORY "/etc"

// IDs de IPCs
#define SM_R11_R14_ID               1
#define SM_R14_R16_ID               2
#define SEM_MUTEX_SM_R11_R14_ID     1
#define SEM_MUTEX_SM_R14_R16_ID     2
#define SEM_R11_CINTA_13            3 
#define SEM_R14_ID                  4
#define SEM_R16_ID                  5

// FIXME: Ver como reemplazar estas constantes por parámetros en 
// el archivo de configuración
// Constantes del sistema 
#define CINTA_13_CAPACITY           3
#define CINTA_15_CAPACITY           5
#define AMOUNT_CINTA_13             2


// FIXME: Refactorizar esto
// Clases y Estructuras generales 
class Caja {
public:
    Caja() {ordenDeCompra_ = 0;};
    Caja(int ordenDeCompra) { ordenDeCompra_ = ordenDeCompra;};
    int getOrdenDeCompra() { return ordenDeCompra_;};
    bool estaVacio() { return ordenDeCompra_ == 0; };
    
private:
    int ordenDeCompra_;
};

#endif /* COMMON_H_ */

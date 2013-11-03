#include "BufferODC.h"


BufferODC::BufferODC() {
}

 
bool BufferODC::getOrdenDeCompra(long key, OrdenDeCompra & odc) {
    it = dictionaryODC_.find(key);
    if (it == dictionaryODC_.end()) {
        return false;
    }
    else {
        odc = it->second;
        return true;
    }
}

bool BufferODC::agregarOrdenDeCompra(long key, OrdenDeCompra odc) {
    it = dictionaryODC_.find(key);
    if (it != dictionaryODC_.end()) {
        return false;
    }
    else {
        dictionaryODC_.insert( std::pair<long, OrdenDeCompra>(key, odc));
        return true;
    }
}

BufferODC::~BufferODC() {
}


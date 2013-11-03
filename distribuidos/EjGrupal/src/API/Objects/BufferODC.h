#ifndef BUFFERODC_H
#define	BUFFERODC_H

#include <map>
#include <iostream>
#include <Common.h>

class BufferODC {
public:
    BufferODC();
    bool getOrdenDeCompra(long key, OrdenDeCompra & odc);
    bool agregarOrdenDeCompra(long key, OrdenDeCompra odc);
    virtual ~BufferODC();
private:
    std::map<long, OrdenDeCompra> dictionaryODC_;
    std::map<long, OrdenDeCompra>::iterator it;

};

#endif	/* BUFFERODC_H */


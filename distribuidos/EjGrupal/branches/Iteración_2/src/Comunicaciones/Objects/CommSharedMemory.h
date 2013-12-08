#ifndef COMMSHAREDMEMORY_H
#define COMMSHAREDMEMORY_H

#include "CommObject.h"
#include <sstream>

template< class T, unsigned int SIZE = sizeof(T) >
class CommSharedMemory : public CommObject {
public:
    CommSharedMemory(std::string name = "")
        : CommObject(name) {}

    void getSharedMemory(const char *fileName, int id) {
        // Get the ID of the CommObject from the ConfigFile
        std::stringstream ss;
        ss << id;

        std::string key;
        key += "shMem-";
        key += fileName;
        key += "-";
        key += ss.str();

        findCommId();
    }

    void read(T* data) const {
        // TODO:
    }

    void write(T* data) {
        // TODO:
    }
};

#endif // COMMSHAREDMEMORY_H

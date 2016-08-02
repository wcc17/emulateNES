//
// Created by Christian Curry on 8/1/16.
//

#ifndef EMULATENES_MEMORY_H
#define EMULATENES_MEMORY_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>

class RAM {

public:
    uint8_t* memory;

    RAM();
    void writeMemoryLocation(uint16_t address, uint8_t value);
    uint8_t readMemoryLocation(uint16_t address);

};


#endif //EMULATENES_MEMORY_H

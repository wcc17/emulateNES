//
// Created by Christian Curry on 8/1/16.
//

#ifndef EMULATENES_MEMORY_H
#define EMULATENES_MEMORY_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include "PPUConstants.h"

class PPU;

class Memory {

public:
    PPU* ppu;

    uint8_t* cpuMemory;

    //16kb ram. 0 - FFFF, but 4000-FFFF is a mirror of $0000-$3FFF
    uint8_t* videoRAM;

    Memory();
    void writeMemoryLocation(uint16_t address, uint8_t value);
    uint8_t readMemoryLocation(uint16_t address);
    uint8_t readVRAM(uint16_t address);
    void writeVRAM(uint16_t address, uint8_t value);
};


#endif //EMULATENES_MEMORY_H
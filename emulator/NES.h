//
// Created by Christian Curry on 5/29/16.
//

#ifndef EMULATE6502_NES_H
#define EMULATE6502_NES_H


#include "CPU.h"
#include "ROM.h"
#include "Mapper.h"

class NES {

public:
    Mapper mapper;

    ROM* rom;
    CPU* cpu;
    RAM* ram;
    NES(CPU* cpu, ROM* rom, RAM* ram);

    bool loadRom();
    void determineMapper(int mapperNumber);
    void initializeMapping();
    void initializeNROM();
};


#endif //EMULATE6502_NES_H

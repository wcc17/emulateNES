//
// Created by Christian Curry on 5/29/16.
//

#ifndef EMULATE6502_NES_H
#define EMULATE6502_NES_H

#include <iostream>
#include "CPU.h"
#include "ROM.h"
#include "PPU.h"
#include "Mapper.h"

class NES {

public:
    NES();
    void start();

private:
    Util util;
    Mapper mapper;

    ROM* rom;
    CPU* cpu;
    RAM* ram;
    PPU* ppu;

    void execute();
    bool loadRom();
    void determineMapper(int mapperNumber);
    void initializeMapping();
    void initializeNROM();
};


#endif //EMULATE6502_NES_H

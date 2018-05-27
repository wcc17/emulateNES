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
    void execute();
    void executeCPU(int cycles);
    void stop();

    ROM* rom;
    CPU* cpu;
    PPU* ppu;

private:
    Util util;

    bool loadRom();
    void raiseInterrupt();
};


#endif //EMULATE6502_NES_H

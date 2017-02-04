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
    void stop();

private:
    Util util;

    ROM* rom;
    CPU* cpu;
    RAM* ram;
    PPU* ppu;

    bool loadRom();
};


#endif //EMULATE6502_NES_H

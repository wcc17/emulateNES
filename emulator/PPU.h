//
// Created by Christian Curry on 6/5/16.
//

#ifndef EMULATENES_PPU_H
#define EMULATENES_PPU_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include "ROM.h"
#include "Memory.h"
#include "CPU.h"

class PPU {

public:
    PPU(Memory* memory, ROM* rom, CPU* cpu);
    void onPowerUp();
    void onReset();
    void execute();
    uint8_t readMemoryLocation(uint16_t address);
    void writeMemoryLocation(uint16_t address, uint8_t data);

private:
    Memory* memory;
    ROM* rom;
    CPU* cpu;
    Util util;

    uint8_t sprRAM[256];

    //for sprites. 64 sprites at 4 bytes each
    uint8_t primaryOAM[256];

    //pallette data
    uint8_t secondaryOAM[32];

    int ppuClockCycle = 0; //0 - 340 (341 clock cycles per 113.667 cpu clock cycles) each clock cycle renders 1 pixel (342 pixels on screen?
    int scanline = 0; //0 - 261 (262 scan lines per frame (each scanline lasts 341 clock cycles)

    //ppu_latch?
    uint16_t ppuLatch;
    uint16_t loopyT; //for temp version of loopyV($2006). does nothing except reload loopyV when applicable
    uint16_t loopyV;

    bool ppuAddrFirstWrite = false; //if false, first write.... if true, second write

    void render();
    void renderScanLine();
    uint8_t getNameTableByte();
    uint16_t getNameTableAddress(uint8_t nameTableSelection);
//    uint8_t getAttributeTableByte();
//    uint16_t getAttributeTableAddress(uint8_t addressTableSelection);
    void handleNMIInterrupt();
//    uint8_t readPatternTable();
};


#endif //EMULATENES_PPU_H

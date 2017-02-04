//
// Created by Christian Curry on 6/5/16.
//

#ifndef EMULATENES_PPU_H
#define EMULATENES_PPU_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include "ROM.h"
#include "Memory.h"

class PPU {

public:
    PPU(Memory* memory, ROM* rom);
    void onPowerUp();
    void onReset();
    void execute();

    void writeRegister(uint16_t address, uint8_t value);
    uint8_t readRegister(uint16_t address);

private:
    Memory* memory;
    ROM* rom;

    //the single letter variable names are unforunate but correspond to whats in the nes docs
    uint16_t v; //current vram address
    uint16_t t; //temp vram address
    uint8_t x; //fine x scroll (3 bit)
    uint8_t w; //write toggle (1 bit) -- will either be 0 or 1
    uint8_t f; //even odd frame flag (1 bit) -- will either be 0 or 1

    uint8_t sprRAM[256];

    //for sprites. 64 sprites at 4 bytes each
    uint8_t primaryOAM[256];

    //pallette data
    uint8_t secondaryOAM[32];

    int ppuClockCycle = 0; //0 - 340 (341 clock cycles per 113.667 cpu clock cycles) each clock cycle renders 1 pixel (342 pixels on screen?
    int scanline = 0; //0 - 261 (262 scan lines per frame (each scanline lasts 341 clock cycles)

    void render();
    void renderScanLine();
    uint8_t getNameTableSelection();
    uint16_t getNameTableAddress(uint8_t nameTableSelection);
};


#endif //EMULATENES_PPU_H

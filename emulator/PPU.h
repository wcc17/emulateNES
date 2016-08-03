//
// Created by Christian Curry on 6/5/16.
//

#ifndef EMULATENES_PPU_H
#define EMULATENES_PPU_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>

class PPU {

    //TODO: $2000-$2007 ARE MIRRORED EVERY 8 BYTES $2008 THROUGH $3FFF

    //VPHB SINN
    /**
     * NMI enable (V)
     * PPU master/slave (P)
     * sprite height (H)
     * background Tile select (B)
     * sprite tile select (S)
     * increment mode (I)
     * nametable select (NN)
     */
    const uint8_t PPU_CTRL = 0x2000;

    //BGRs bMmG
    /**
     * color emphasis (BGR)
     * sprite enable (s)
     * background enable (b)
     * sprite left column enable (M)
     * background left column enable (m)
     * greyscale (G)
     */
    const uint8_t PPU_MASK = 0x2001;

    //VSO- ----
    /**
     * vblank (v)
     * sprite 0 hit (S)
     * sprite overflow (O)
     * read resets write pair for $2005/$2006
     */
    const uint8_t PPU_STATUS = 0x2002;

    //Object attribute memory
    //aaaa aaaa
    //OAM read/write address
    const uint8_t OAM_ADDR = 0x2003;

    //Object attribute data
    //dddd dddd
    //OAM data read/write
    const uint8_t OAM_DATA = 0x2004;

    //xxxx xxxx
    //fine scroll position (two writes: X, Y)
    const uint8_t PPU_SCROLL = 0x2005;

    //aaaa aaaa
    //PPU read/write address (two writes, MSB, LSB)
    const uint8_t PPU_ADDR = 0x2006;

    //dddd dddd
    //PPU data read/write
    const uint8_t PPU_DATA = 0x2007;

    //aaaa aaaa
    //OAM DMA high address
    const uint8_t OAM_DMA = 0x4014;

    //16kb ram. 0 - FFFF, but 4000-FFFF is a mirror of $0000-$3FFF
    uint8_t videoRAM[8000];

    //for sprites. 64 sprites at 4 bytes each
    uint8_t objectAttributeMemory[256];

    int ppuClockCycle = 0; //0 - 340 (341 clock cycles per 113.667 cpu clock cycles) each clock cycle renders 1 pixel (342 pixels on screen?
    int scanline = 0; //0 - 261 (262 scan lines per frame (each scanline lasts 341 clock cycles)

    //functions
    void render();
    void renderScanLine();
    uint8_t readVRAM(uint16_t address);
    void writeVRAM(uint16_t address, uint8_t value);
};


#endif //EMULATENES_PPU_H

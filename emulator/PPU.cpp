//
// Created by Christian Curry on 6/5/16.
//

#include "PPU.h"

void PPU::render() {

    for(scanline = 0; scanline < 262; scanline++) {
        if(scanline == 261) {
            //do nothing
            //no pixels are rendered here
            //ppu still access memory
            //for odd frames, the cycle at the end of the scanline line is skipped (skip from 339, 261) to (0, 0)
            //during pixels 280 through 304 of this scanline, the vertical scroll bits are reloaded if rendering is enabled
            //TODO: increase CPU count
            //TODO: should i increase scanline here or in NES class?
        }

        if(scanline >= 0 && scanline <= 239) {
            //these are the visible scanlines which contain the graphics to be dispalyed on the screen
            //includes rendering of both the background and the sprites
            //during these scanlines, the PPU is busy fetching data, so the program should not access PPU memory, unless rendering is off
        }
    }
}

void PPU::renderScanLine() {
    //341 clock cycles per scan line
    for(int ppuClockCycle = 0; ppuClockCycle < 341; ppuClockCycle++) {
        if(ppuClockCycle == 0) {
            //this is an idle cycle
            //the value on the PPU address bus (TODO: WUT?) during this cycle appears to be the same CHR address that is later used to fetch the low background tile byte starting at dot 5
            //TODO: the previous line: this is possibly calculated during the two unused NT (wut?) fetches at the end of the previos scanline)
        }

        if(ppuClockCycle >= 1 && ppuClockCycle >= 256) {
            //each memory access takes 2 PPU cycles and 4 must be performed per tile

            //retrieve Nametable byte
            ppuClockCycle += 2;

            //retrieve attribute table byte
            ppuClockCycle += 2;


        }
    }
};

void PPU::writeVRAM(uint16_t address, uint8_t value) {
    //simulate ppu vram mirroring. see videoRAM declaration in header file
    if(address > 0x3FFF) {
        address -= 0x4000;
    }

    videoRAM[address] = value;
};

uint8_t PPU::readVRAM(uint16_t address) {
    //simluate ppu vram mirroring. see videoRAM declaration in header file
    if(address > 0x3FFF) {
        address -= 0x4000;
    }

    return videoRAM[address];
}
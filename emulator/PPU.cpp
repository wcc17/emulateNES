//
// Created by Christian Curry on 6/5/16.
//

#include "PPU.h"

PPU::PPU(Memory* memory, ROM* rom) {
    this->memory = memory;
    this->rom = rom;
    memory->ppu = this;
}

void PPU::onPowerUp() {
    //? = unknown
    //x = irrelevant
    //+ = often set
    //U = unchanged
    memory->writeVRAM(PPU_CTRL, 0x00);
    memory->writeVRAM(PPU_MASK, 0x00);
    memory->writeVRAM(PPU_STATUS, 0xA0); //+0+x xxxx
    memory->writeVRAM(OAM_ADDR, 0x00);
    //TODO: $2005 (PPUSCROLL) and $2006 (PPUADDR) latches are cleared
    memory->writeVRAM(PPU_SCROLL, 0x00);
    memory->writeVRAM(PPU_ADDR, 0x00);
    memory->writeVRAM(PPU_DATA, 0x00);
    //TODO: odd frame????
    //TODO: OAM -- pattern
    //TODO: NT RAM (external, in Control Deck)	mostly $FF
    //TODO: CHR RAM (external, in Game Pak)	unspecified pattern
}

void PPU::onReset() {
    //? = unknown
    //x = irrelevant
    //+ = often set
    //U = unchanged
    memory->writeVRAM(PPU_CTRL, 0x00);
    memory->writeVRAM(PPU_MASK, 0x00);
    memory->writeVRAM(PPU_STATUS, (memory->readVRAM(PPU_STATUS) & 0x80)); //U??x xxxx
    memory->writeVRAM(OAM_ADDR, 0x00);
    //TODO: $2005 (PPUSCROLL) and $2006 (PPUADDR) latches are cleared
    memory->writeVRAM(PPU_SCROLL, 0x00);
    memory->writeVRAM(PPU_DATA, 0x00);
    //TODO: odd frame????
    //TODO: OAM -- pattern
    //TODO: NT RAM (external, in Control Deck)	unchanged
    //TODO: CHR RAM (external, in Game Pak)	unchanged

}

void PPU::execute() {
    //YEAHHHHHHh
}

void PPU::render() {
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

void PPU::renderScanLine() {
    //341 clock cycles per scan line
    if(ppuClockCycle == 0) {
        //this is an idle cycle
        //the value on the PPU address bus (TODO: WUT?) during this cycle appears to be the same CHR address that is later used to fetch the low background tile byte starting at dot 5
        //TODO: the previous line: this is possibly calculated during the two unused NT (wut?) fetches at the end of the previos scanline)
    }

    if(ppuClockCycle >= 1 && ppuClockCycle >= 256) {
        //each memory access takes 2 PPU cycles and 4 must be performed per tile

        //retrieve Nametable byte
        uint8_t nameTableSelection = getNameTableSelection();
        uint16_t nameTableAddress = getNameTableAddress(nameTableSelection);
        ppuClockCycle += 2;

        //retrieve attribute table byte
        ppuClockCycle += 2;


    }
};

uint16_t PPU::getNameTableAddress(uint8_t nameTableSelection) {
    if(rom->horizontalMirroring) {
        //$2000 equals $2400 and $2800 equals $2C00
        switch(nameTableSelection) {
            case 0x00:
                return 0x2000;
            case 0x01:
                return 0x2000;
            case 0x02:
                return 0x2800;
            case 0x03:
                return 0x2800;
            default:
                printf("This is a problem\n");
        }
    } else if(rom->verticalMirroring) {
        //$2000 equals $2800 and $2400 equals $2c00
        switch(nameTableSelection) {
            case 0x00:
                return 0x2000;
            case 0x01:
                return 0x2400;
            case 0x02:
                return 0x2000;
            case 0x03:
                return 0x2400;
            default:
                printf("This is a problem\n");
        }
    } else {
        switch(nameTableSelection) {
            case 0x00:
                return 0x2000;
            case 0x01:
                return 0x2400;
            case 0x02:
                return 0x2800;
            case 0x03:
                return 0x2c00;
            default:
                printf("This is a problem\n");
        }
    }
}

uint8_t PPU::getNameTableSelection() {

    //last two bits of PPU_CTRL are the name table select.
    //00000011 AND whatever8bitvalue will give the selection we want
    uint8_t selection = 0x03 & memory->readMemoryLocation(PPU_CTRL);

    //TODO: WHAT WAS THIS FOR
    if(selection > 0x03) {
        printf("THIS IS A PROBLEM\n");
    }

    return selection;
}

void PPU::writeRegister(uint16_t address, uint8_t value) {

}

uint8_t PPU::readRegister(uint16_t address) {
    switch(address) {
        case PPU_STATUS:
            //TODO: this isn't the only thing that happens
            w = 0;
            return memory->cpuMemory[PPU_STATUS];
    }
}
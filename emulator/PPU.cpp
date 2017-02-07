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
    memory->writeMemoryLocation(PPU_CTRL, 0x00);
    memory->writeMemoryLocation(PPU_MASK, 0x00);
    memory->writeMemoryLocation(PPU_STATUS, 0xA0); //+0+x xxxx
    memory->writeMemoryLocation(OAM_ADDR, 0x00);
    //TODO: $2005 (PPUSCROLL) and $2006 (PPUADDR) latches are cleared
    memory->writeMemoryLocation(PPU_SCROLL, 0x00);
    memory->writeMemoryLocation(PPU_ADDR, 0x00);
    memory->writeMemoryLocation(PPU_DATA, 0x00);

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
    memory->writeMemoryLocation(PPU_CTRL, 0x00);
    memory->writeMemoryLocation(PPU_MASK, 0x00);
    memory->writeMemoryLocation(PPU_STATUS, (memory->readVRAM(PPU_STATUS) & 0x80)); //U??x xxxx
    memory->writeMemoryLocation(OAM_ADDR, 0x00);
    //TODO: $2005 (PPUSCROLL) and $2006 (PPUADDR) latches are cleared
    memory->writeMemoryLocation(PPU_SCROLL, 0x00);
    memory->writeMemoryLocation(PPU_DATA, 0x00);

    //TODO: odd frame????
    //TODO: OAM -- pattern
    //TODO: NT RAM (external, in Control Deck)	unchanged
    //TODO: CHR RAM (external, in Game Pak)	unchanged

}

void PPU::execute() {
    //YEAHHHHHHh
}

void PPU::render() {

    if(scanline < 240) {
        //visible scanlines (0 - 239)
    } else if(scanline < 241) {
        //post render scanline (240)
    } else if(scanline < 261) {
        //vertical blanking lines(241-260)
    } else if(scanline == 261) {
        //pre-render scanline (261)
    }




}

void PPU::renderScanLine() {

    //Some general rules
    //External PPU memory can be accessed every 2 PPU clock cycles
    //With 341 clock cycles per scanline, the PPU makes 170 memory addresses per scanline TODO: can be used to test that things are working properly
    //After the 170th fetch, the PPU does nothing for 1 clock cycle.
    //A single pixel is rendered every clock cycle

    //1. Memory fetch phase 1 thru 128
    //This process is repeated 32 times (32 tiles in a scanline)
    //Each of these 4 things takes 1 clock cycle each,
    //all repeated 32 times = 128 clock cycles in 1 scanline
    //1. Fetch name table byte
    //2. Attribute table byte
    //3. Pattern table bitmap #0
    //4. Pattern table bitmap #1

    if(ppuClockCycle == 0) {
        //idle cycle
    } else {
        int phase = ppuClockCycle % 4;
        switch(phase) {
            case 0:
                //fetch name table byte
                uint8_t nameTableByte = getNameTableByte();
                break;
            case 1:
                //fetch attribute table byte
                break;
            case 2:
                //fetch pattern table bitmap #0
                break;
            case 3:
                //fetch pattern table bitmap #1
                break;
        }
    }
}

uint8_t PPU::getNameTableByte() {
    //last two bits of PPU_CTRL are the name table select.
    //00000011 AND whatever8bitvalue will give the selection we want
    uint8_t selection = 0x03 & memory->readMemoryLocation(PPU_CTRL);

    if(selection > 0x03) {
        printf("Name table selection should only be 0-3\n");
    }

    return memory->readVRAM(getNameTableAddress(selection));
}

uint16_t PPU::getNameTableAddress(uint8_t nameTableSelection) {
    uint16_t address;

    if(rom->horizontalMirroring) {
        //$2000 equals $2400 and $2800 equals $2C00
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2000;
                break;
            case 0x02:
                address = 0x2800;
                break;
            case 0x03:
                address = 0x2800;
                break;
        }
    } else if(rom->verticalMirroring) {
        //$2000 equals $2800 and $2400 equals $2c00
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2400;
                break;
            case 0x02:
                address = 0x2000;
                break;
            case 0x03:
                address = 0x2400;
                break;
        }
    } else {
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2400;
                break;
            case 0x02:
                address = 0x2800;
                break;
            case 0x03:
                address = 0x2c00;
                break;
        }
    }

    uint16_t vramAddress = v; //TODO: still not 100% where v is coming from yet
    address = address | (vramAddress & 0x0FFF); //TODO: why 0x0FFF? offset?
    return address;
}

void PPU::writeRegister(uint16_t address, uint8_t value) {

}

uint8_t PPU::readRegister(uint16_t address) {
    switch(address) {
        case PPU_STATUS:
            //TODO: this isn't the only thing that happens
            w = 0;
            return memory->readMemoryLocation(PPU_STATUS);
    }
}
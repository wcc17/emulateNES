//
// Created by Christian Curry on 8/1/16.
//

#include "Memory.h"
#include "PPU.h"

Memory::Memory() {
    cpuMemory = new uint8_t[65535];
    ppuMemory = new uint8_t[0x4000];
}

void Memory::writeMemoryLocation(uint16_t address, uint8_t value) {
    if(address <= 0x1FFF) {
        uint16_t mirroredAddress = address & 0x07FF;
        cpuMemory[mirroredAddress] = value;
    } else if(address > 0x2000 && address <= 0x3FFF) {
        uint16_t mirroredAddress = 0x2000 + (address & 0x07);
        ppu->writeMemoryLocation(mirroredAddress, value);
    } else {
        cpuMemory[address] = value;
    }
}

uint8_t Memory::readMemoryLocation(uint16_t address) {
    if(address < 0x2000 || address > 0x7FFF) {
        return cpuMemory[address];
    }

    //TODO: other stuff happens to ppu registers here
    return ppu->readMemoryLocation(address);
}

void Memory::directWriteMemoryLocation(uint16_t address, uint8_t value) {
    cpuMemory[address] = value;
}

uint8_t Memory::directReadMemoryLocation(uint16_t address) {
    return cpuMemory[address];
}

void Memory::writeVRAM(uint16_t address, uint8_t value) {
    //$3000 - $3EFF mirrors $2000-$2EFF
    if(address >= 0x3000 && address < 0x3F00) {
        address -= 0x1000;
    }

    //0-223                  //0-31
    //$3F20-$3FFF mirrors $3F00-$3F1F
    if(address >= 0x3F20 && address < 0x4000) {
        address %= 32;
    }

    //simulate ppu vram mirroring. see ppuMemory declaration in header file
    if(address > 0x3FFF) {
        address -= 0x4000;
    }

    ppuMemory[address] = value;
};

uint8_t Memory::readVRAM(uint16_t address) {
    return ppuMemory[address];
}
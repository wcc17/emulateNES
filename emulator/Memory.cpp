//
// Created by Christian Curry on 8/1/16.
//

#include "Memory.h"
#include "PPU.h"
#include "CPU.h"

Memory::Memory() {
    cpuMemory = new uint8_t[65535];
    videoRAM = new uint8_t[0x4000];
}

void Memory::writeMemoryLocation(uint16_t address, uint8_t value) {
    if(address <= 0x1FFF) {
        //RAM
        uint16_t mirroredAddress = address & 0x07FF;
        cpuMemory[mirroredAddress] = value;
    } else if(address >= 0x2008 && address <= 0x3FFF) {
        uint16_t mirroredAddress = 0x2000 + (address + 0x07);
//        cpuMemory[mirroredAddress] = value;
        ppu->writeRegister(mirroredAddress, value);
    } else {
        //ANYTHING ELSE
        cpuMemory[address] = value;
    }
}

uint8_t Memory::readMemoryLocation(uint16_t address) {
    if(address <= 0x1FFF) {
        uint16_t mirroredAddress = address & 0x07FF;
        return cpuMemory[mirroredAddress];
    } else if(address >= 0x2008 && address <= 0x3FFF) {
        uint16_t mirroredAddress = 0x2000 + (address + 0x07);
//        return cpuMemory[mirroredAddress];
        return ppu->readRegister(mirroredAddress);
    } else {
        return cpuMemory[address];
    }
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

    //simulate ppu vram mirroring. see videoRAM declaration in header file
    if(address > 0x3FFF) {
        address -= 0x4000;
    }

    videoRAM[address] = value;
};

uint8_t Memory::readVRAM(uint16_t address) {
    //$3000 - $3EFF mirrors $2000-$2EFF
    if(address >= 0x3000 && address < 0x3F00) {
        address -= 0x1000;
    }

    //0-223                  //0-31
    //$3F20-$3FFF mirrors $3F00-$3F1F
    if(address >= 0x3F20 && address < 0x4000) {
        address %= 32;
    }

    //simluate ppu vram mirroring. see videoRAM declaration in header file
    if(address > 0x3FFF) {
        address -= 0x4000;
    }

    return videoRAM[address];
}
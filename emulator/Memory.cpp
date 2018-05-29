//
// Created by Christian Curry on 8/1/16.
//

#include "Memory.h"
#include "PPU.h"

Memory::Memory() {
    cpuMemory = new uint8_t[65536];
    ppuMemory = new uint8_t[16384];

    for(int i = 0; i < 65536; i++) {
        cpuMemory[i] = 0;
    }
    for(int i = 0; i < 16384; i++) {
        ppuMemory[i] = 0;
    }

}

void Memory::writeMemoryLocation(uint16_t address, uint8_t value) {
    if(address < 0x2000) {
        uint16_t mirroredAddress = address & 0x07FF;
        directWriteMemoryLocation(mirroredAddress, value);
    } else if(address > 0x2000 && address <= 0x3FFF) {
        uint16_t mirroredAddress = address & 0x2007;
        ppu->writeMemoryLocation(mirroredAddress, value);
    } else {
        directWriteMemoryLocation(address, value);
    }
}

uint8_t Memory::readMemoryLocation(uint16_t address) {
    if(address < 0x2000) {
        uint16_t mirroredAddress = address & 0x07ff;
        return directReadMemoryLocation(mirroredAddress);
    } else if(address >= 0x2000 && address <= 0x3fff) {
        uint16_t mirroredAddress = address & 0x2007;
        return ppu->readMemoryLocation(mirroredAddress);
    } else {
        return directReadMemoryLocation(address);
    }
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
    //TODO: copied code, make mirroring its own method
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

    return ppuMemory[address];
}
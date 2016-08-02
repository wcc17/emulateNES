//
// Created by Christian Curry on 8/1/16.
//

#include "RAM.h"

RAM::RAM() {
    memory = new uint8_t[65535];
}

void RAM::writeMemoryLocation(uint16_t address, uint8_t value) {
    if(address <= 0x1FFF) {
        //RAM
        uint16_t mirroredAddress = address & 0x07FF;
        memory[mirroredAddress] = value;
    } else if(address >= 0x2008 && address <= 0x3FFF) {
        //IO REGISTERS
        uint16_t mirroredAddress = 0x2000 + (address + 0x07);
        memory[mirroredAddress] = value;
    } else {
        //ANYTHING ELSE
        memory[address] = value;
    }
}

uint8_t RAM::readMemoryLocation(uint16_t address) {
    if(address <= 0x1FFF) {
        uint16_t mirroredAddress = address & 0x07FF;
        return memory[mirroredAddress];
    } else if(address >= 0x2008 && address <= 0x3FFF) {
        uint16_t mirroredAddress = 0x2000 + (address + 0x07);
        return memory[mirroredAddress];
    } else {
        return memory[address];
    }
}
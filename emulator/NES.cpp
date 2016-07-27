//
// Created by Christian Curry on 5/29/16.
//

#include "NES.h"

NES::NES(CPU* cpu, Rom* rom) {
    this->cpu = cpu;
    this->rom = rom;
}

//will return false if rom is not loaded correctly
bool NES::loadRom() {
    bool romValidated = rom->validateRom();

    if(romValidated) {
        rom->getRomInformation();
    } else {
        std::cout << "Invalid rom!" << std::endl;
        return false;
    }

    determineMapper(rom->mapperNumber);
    initializeMapping();

    return true;
}

void NES::determineMapper(int mapperNumber) {
    switch(mapperNumber) {
        case 0:
            mapper = NROM;
            break;
        case 1:
            mapper = MMC1;
            break;
        case 2:
            mapper = UNROM;
            break;
        case 3:
            mapper = CNROM;
            break;
        case 4:
            mapper = MMC3;
            break;
        case 5:
            mapper = MMC5;
            break;
    }
}

void NES::initializeMapping() {
    switch(mapper) {
        case NROM:
            initializeNROM();
            break;
        case MMC1:
            break;
        case UNROM:
            break;
        case CNROM:
            break;
        case MMC3:
            break;
        case MMC5:
            break;
    }
}

//TODO: use memcpy instead of for loops
void NES::initializeNROM() {
    uint16_t romStart = sizeof(rom->header);

    //if there is only one PRG-ROM block, we need to mirror it at C000-FFFF from 8000-BFFF
    uint16_t romIndex = romStart;
    if(rom->prgRom16KBanks == 1) {
        for(uint16_t i = 0x8000; i <= 0xBFFF; i++) {
            cpu->writeMemoryLocation(i, rom->memory[romIndex]);
            cpu->writeMemoryLocation(i + 0x4000, rom->memory[romIndex]);
            romIndex++;
        }
    } else {
        for(uint16_t i = 0x8000; i<= 0xFFFF; i++) {
            cpu->writeMemoryLocation(i, rom->memory[romIndex]);
            romIndex++;
        }
    }

    //copy CHR pages into CPU memory and PPU memory
    romStart = romIndex;
    for(int i = 0; i < rom->header.chrRom8KBanks; i++) {
        //this will just be mirrored memory
        //TODO: copy the first one into the PPU memory
        if(i == 0) {
            //copy into PPU memory
        }


    }

    std::cout << "break" << std::endl;
}
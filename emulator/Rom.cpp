//
// Created by Christian Curry on 6/4/16.
//

#include <fstream>
#include <vector>
#include "ROM.h"
#include "PPU.h"

void ROM::readRom(std::string fileName) {
    std::fstream file(fileName, std::fstream::in);
    printf(strerror(errno));

    char ch;
    int i = 0;
    while(file >> std::noskipws >> ch) {
        uint8_t byte = ch;
        romCache[i++] = byte;
    }

    file.close();
}

bool ROM::validateRom() {
    memcpy(&header, romCache, sizeof(header));

    if(memcmp(header.signature, "NES\x1A", 4)) {
        return false;
    }

    return true;
}

void ROM::getRomInformation() {
    Util util;

    //see the declaration in Rom.h for what all of these mean
    prgRom16KBanks = header.prgRom16KBanksSize;
    chrRom8KBanks = header.chrRom8KBanksSize;

    bool mirroringType = util.checkBit(header.romControlByte1, 0);
    if(mirroringType) {
        verticalMirroring = true;
    } else {
        horizontalMirroring = true;
    }

    SRAMEnabled = util.checkBit(header.romControlByte1, 1);
    _512ByteTrainerPresent = util.checkBit(header.romControlByte1, 2);
    fourScreenVRAMLayout = util.checkBit(header.romControlByte1, 3);

    //put the high bits of the romControlBytes that represent the mapperNumber
    //romControlByte1 goes to the lower bits of the mapper number
    //romControlByte2 goes to the higher bits of the mapper number
    uint8_t lowerMapperNumber = header.romControlByte1 & 0xF0;  //0xF0 = 240 = 11110000
    uint8_t higherMapperNumber = header.romControlByte2 & 0xF0;
    mapperNumber = higherMapperNumber | (lowerMapperNumber >> 4);

    ram8KBanks = header.reserved[0] + 1; //TODO: FIGURE OUT WHAT THIS IS. should be number of 8kb ram banks, when 0, assume there is 1 8kb ram banks. when 1, is there 2?
}

void ROM::determineAndSetMapper(int mapperNumber) {
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

void ROM::initializeMapping(Memory* memory) {
    switch(mapper) {
        case NROM:
            initializeNROM(memory);
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

void ROM::initializeNROM(Memory* memory) {
    uint16_t romStart = sizeof(this->header);

    //if there is only one PRG-ROM block, we need to mirror it at C000-FFFF from 8000-BFFF
    uint16_t romIndex = romStart;
    if(this->prgRom16KBanks == 1) {
        //start at cpu memory 0x8000, start at rom memory 0x0016, copy over 4000 bytes (16384 to hex = 4000)
        memcpy(memory->cpuMemory + 0x8000, this->romCache + romStart, 16384);
        memcpy(memory->cpuMemory + 0xC000, this->romCache + romStart, 16384); //mirror starting 0xC000
    } else {
        //map 2x 16kb, first one into 8000, second into c000 (no mirroring starting at 0xC000 basically
        memcpy(memory->cpuMemory + 0x8000, this->romCache + romStart, 16384);
        memcpy(memory->cpuMemory + 0xC000, this->romCache + romStart + ((prgRom16KBanks - 1) * 16384), 16384);
    }

    util.printMemory(0x0000, 0xFFFF, memory->cpuMemory);

    //copy CHR pages into PPU memory
    for(int i = 0; i < 16384; i++) {
        memory->ppuMemory[i] = 0;
    }
//    util.printMemory(0x0000, 0x4000, memory->ppuMemory);
//    util.printMemory(0x0000, 0xFFFF, this->romCache);
    memcpy(memory->ppuMemory, this->romCache + romStart + (this->prgRom16KBanks * 16384), 8192);

    //get title from last 128 bytes
    //memcpy(title, this->romCache + romStart + (prgRom16KBanks * 16384) + 8192, 128);

    //TODO: what do i do with these:
    //$2000-2FFF is normally mapped to the 2kB NES internal VRAM, providing 2 nametables with a mirroring configuration controlled by the cartridge, but it can be partly or fully remapped to RAM on the cartridge, allowing up to 4 simultaneous nametables.


//    util.printMemory(0x0000, 0x4000, memory->ppuMemory);
}
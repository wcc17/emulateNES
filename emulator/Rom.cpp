//
// Created by Christian Curry on 6/4/16.
//

#include <fstream>
#include <vector>
#include "ROM.h"
#include "Util.h"

void ROM::readRom(std::string fileName) {
    std::fstream file(fileName, std::fstream::in);

    char ch;
    int i = 0;
    while(file >> std::noskipws >> ch) {
        uint8_t byte = ch;
        memory[i++] = byte;
    }

//    uint8_t byte;
//    while (file >> std::hex >> byte) {
//        bytes.push_back(byte);
//    }
//
//    for(int i = 0; i < bytes.size(); i++) {
//        byte = bytes[i];
//        memory[i] = byte;
//    }

    file.close();
}

bool ROM::validateRom() {
    memcpy(&header, memory, sizeof(header));

    if(memcmp(header.signature, "NES\x1A", 4)) {
        return false;
    }

    return true;
}

void ROM::getRomInformation() {
    Util util;

    //see the declaration in Rom.h for what all of these mean
    prgRom16KBanks = header.prgRom16KBanks;
    chrRom8KBanks = header.chrRom8KBanks;

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
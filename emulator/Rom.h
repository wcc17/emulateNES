//
// Created by Christian Curry on 6/4/16.
//

#ifndef EMULATENES_ROM_H
#define EMULATENES_ROM_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <iostream>
#include "Mapper.h"
#include "ROM.h"
#include "Util.h"
#include "CPU.h"


class ROM {

public:
    Mapper mapper;

    //this is only used to hold the rom when its first loaded. eventually will be loaded into CPU/PPU memory
    uint8_t romCache[0x100000];

    //iNES 2.0 header info:
    //http://nesdev.com/iNES.txt
    struct {
        uint8_t signature[4];
        uint8_t prgRom16KBanksSize;    //size of prg rom in 16kb unis (1 == 1 * 16kb)
        uint8_t chrRom8KBanksSize;      //size of chr rom in 8kb units
        uint8_t romControlByte1;
        uint8_t romControlByte2;
        uint8_t prgRamSize; //size of prg ram in 8kb units (value 9 infers 8
        uint8_t romControlByte3;
        uint8_t romControlByte4; //says unofficial on the wiki
        uint8_t reserved[5];

    } header;

    int prgRom16KBanks; //the area of ROM used to store program code
    int chrRom8KBanks;
    bool verticalMirroring = false;
    bool horizontalMirroring = false;
    bool SRAMEnabled = false;   //indicates the presence of battery-backed RAM at &6000-$7FFF
    bool _512ByteTrainerPresent = false;
    bool fourScreenVRAMLayout = false;
    int mapperNumber;
    int ram8KBanks;

    void readRom(std::string fileName);
    bool validateRom();
    void getRomInformation();
    void determineAndSetMapper(int mapperNumber);
    void initializeMapping(Memory* memory);
    void initializeNROM(Memory* memory);

    Util util;
};


#endif //EMULATENES_ROM_H

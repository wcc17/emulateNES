//
// Created by Christian Curry on 6/4/16.
//

#ifndef EMULATENES_ROM_H
#define EMULATENES_ROM_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <iostream>


class ROM {

public:

    //this is only used to hold the rom when its first loaded. eventually will be loaded into CPU/PPU memory
    uint8_t memory[0x100000];

    //iNES 2.0 header info:
    //http://nesdev.com/iNES.txt
    struct {
        uint8_t signature[4];
        uint8_t prgRom16KBanks;    //16kb rom banks
        uint8_t chrRom8KBanks;      //8kb vrom banks
        uint8_t romControlByte1;
        uint8_t romControlByte2;
        uint8_t reserved[8];

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
};


#endif //EMULATENES_ROM_H
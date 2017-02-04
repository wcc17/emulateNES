//
// Created by Christian Curry on 5/29/16.
//

#include "NES.h"

using namespace std;

NES::NES() {
    //set up RAM
    ram = new RAM();

    //set up CPU
    cpu = new CPU(ram);
    cpu->debug = true;

    //set up ROM
    rom = new ROM();
    rom->readRom("sample_programs/nestest.nes");

    //set up PPU
    ppu = new PPU(ram, rom);
}

void NES::start() {
    cpu->onPowerUp();
    ppu->onPowerUp();

    bool romLoaded = loadRom();
    if(romLoaded) {
//        util.printMemory(0x0000, 0xFFFF, cpu->ram->memory);

        //TODO: will not be where PC starts. at least will not be handled here
        cpu->programCounter = 0xc000;
    } else {
        //TODO: needs to be handled better
        cout << "error loading rom!" << endl;
    }
}

void NES::execute() {
//for debug, uncomment
//      cin.ignore();

    cpu->execute();
}

void NES::stop() {
    cout << endl;
    cout << "Final PC: " << hex << setw(4) << cpu->programCounter << endl << endl;
    util.printMemory(0x0000, 0xFFFF, cpu->ram->memory);
    util.printStack(cpu->ram->memory);
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

    rom->determineMapper(rom->mapperNumber);
    rom->initializeMapping(cpu);

    return true;
}
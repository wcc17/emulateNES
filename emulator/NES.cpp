//
// Created by Christian Curry on 5/29/16.
//

#include "NES.h"

using namespace std;

NES::NES() {
    //set up Memory
    Memory* memory = new Memory();

    //set up CPU
    cpu = new CPU(memory);
    cpu->debug = true;

    //set up ROM
    rom = new ROM();
    rom->readRom("sample_programs/nestest.nes");

    //set up PPU
    ppu = new PPU(memory, rom);
}

void NES::start() {
    cpu->onPowerUp();
    ppu->onPowerUp();

    bool romLoaded = loadRom();
    if(romLoaded) {
        //util.printMemory(0x0000, 0xFFFF, cpu->ram->memory);

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

    //1 CPU cycle ~= 3 PPU cycles
    int cycle = 0;
    while(cycle < 3) {
        ppu->execute();
        cycle++;
    }

    cpu->execute();
}

void NES::stop() {
    cout << endl;
    cout << "Final PC: " << hex << setw(4) << cpu->programCounter << endl << endl;
    util.printMemory(0x0000, 0xFFFF, cpu->memory->cpuMemory);
    util.printStack(cpu->memory->cpuMemory);
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
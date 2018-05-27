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
    ppu = new PPU(memory, rom, cpu);
}

void NES::start() {
    cpu->onPowerUp();
//    ppu->onPowerUp();

    bool romLoaded = loadRom();
    if(romLoaded) {
        //util.printMemory(0x0000, 0xFFFF, cpu->ram->memory);

        //TODO: PC will start here, should it be moved to CPU reset/init?
        cpu->programCounter = 0xc004;
    } else {
        //TODO: needs to be handled better
        cout << "error loading rom!" << endl;
    }
}

void NES::execute() {
    //There are supposed to be rougly (114 cpu cycles)*(262 scanlines perframe) = 29868 every second (114 cpu cycles per scanline)
    int frameCycleGoal = (114 * 262);

    do {
        //for debug, uncomment
        //      cin.ignore();

        if(frameCycleGoal % 100 == 0) {
            printf("break");
        }

        //for the very first scanline want something to render
        if(cpu->cycleGoal == 0) {
            cpu->cycleGoal = 114;
            ppu->execute(cpu->cycleGoal);
            cpu->cycleGoal = 0;
        } else {
            ppu->execute(cpu->cycleGoal);
        }

        //do enough cpu cycles to do one ppu scanline
        int cyclesForScanline = 114;
        do {
            cpu->execute();
            cyclesForScanline -= cpu->cyclesToExecute;
            cpu->cycleGoal += cpu->cyclesToExecute;
            cpu->cyclesToExecute = 0;
        } while (cyclesForScanline > 0);

        cpu->totalCycles += cpu->cycleGoal; //add cycles executed here to overall count (will be at least one scanlines worth)
        frameCycleGoal -= cpu->cycleGoal;

        cpu->cycleGoal = 0;
    } while (frameCycleGoal > 0);
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

    rom->determineAndSetMapper(rom->mapperNumber);
    rom->initializeMapping(cpu->memory); //could also be subbed with ppu->memory. stupid

    return true;
}

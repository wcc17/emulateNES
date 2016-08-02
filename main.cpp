#include <iostream>
#include "ROM.h"
#include <NES.h>
#include "emulator/Assembler.h"
#include "Emulator.h"

using namespace std;

void printMemory(uint16_t start, uint16_t end, CPU* cpu) {
    int x = 0;
    printf("%04x:  ", start);

    //printf("%04x:  ", start);
    for(int i = start; i <= end; i++) {
        printf("%02x  ", cpu->ram->memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
            printf("%04x:  ", i+1);
        }

    }

    cout << endl;
}

void printStack(CPU* cpu) {
    int x = 0;

    for(int i = 0x0100; i <= 0x01ff; i++) {
        printf("%02x ", cpu->ram->memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
        }
    }
}

void printDebugInformation(CPU* cpu) {
    Util util;

    cout << "Results of previous instruction: " << endl;
    //display results of previous instruction and argument
    cout << "Program Counter: ";
    util.printWord(cpu->programCounter);
    cout << endl;

    cout << "Accumulator: ";
    util.printByte(cpu->accumulator);
    cout << endl;

    cout << "xIndex: ";
    util.printByte(cpu->xIndex);
    cout << endl;

    cout << "yIndex: ";
    util.printByte(cpu->yIndex);
    cout << endl;

    cout << "Stack Pointer: ";
    util.printByte(cpu->stackPointer);
    cout << endl;

    cout << "N V - B D I Z C" << endl;
    printf("%d %d %d %d %d %d %d %d\n", cpu->flags.negative, cpu->flags.overflow, cpu->flags.ignored,
           cpu->flags.breakFlag, cpu->flags.decimal, cpu->flags.interrupt, cpu->flags.zero, cpu->flags.carry);

    cout << "Stack: " << endl;
    printStack(cpu);
    cout << endl;

//    cout << "Next instruction results to be printed: " << endl << endl;
}

int main() {
    //    Assembler assembler(cpu);
//    string fileName = "sample_programs/testSubroutines2.asm";
//    assembler.readFile(fileName.c_str());

    RAM* ram = new RAM();

    CPU *cpu = new CPU(ram);
    cpu->debug = true;

    ROM *rom = new ROM();
    rom->readRom("sample_programs/nestest.nes");

    NES *nes = new NES(cpu, rom, ram);
    bool romLoaded = nes->loadRom();

    printMemory(0x0000, 0xFFFF, cpu);

    if(romLoaded) {
        bool debug = false;
        cpu->programCounter = 0xc000;

        int i = 0;
        while(i < 9000) {

            if(debug) {
                cin.ignore();
            }

            cpu->execute();
            i++;
        }

        cout << endl;
        cout << "Final PC: " << hex << setw(4) << cpu->programCounter << endl << endl;
        printMemory(0x0000, 0xFFFF, cpu);
        cout << endl;
        printStack(cpu);
    }
    return 0;

//    Emulator emulator;
//
//    return emulator.onExecute();
}
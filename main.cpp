#include <iostream>
#include "emulator/Assembler.h"
#include "emulator/Util.h"
#include "emulator/CPU.h"
#include "test_emulator/cpu_tests/CPUTest.h"

using namespace std;

void printMemory(uint16_t start, uint16_t end, CPU* cpu) {
    int x = 0;
    printf("%04x:  ", start);

    //printf("%04x:  ", start);
    for(int i = start; i <= end; i++) {
        printf("%02x  ", cpu->memory[i]);

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
        printf("%02x ", cpu->memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
        }
    }
}

void readBinaryFile(CPU* cpu, std::string fileName) {
    ifstream file;
    vector<uint8_t> bytes;   //just going to let bytes vector increase by itself rather tahn trying to reserve space for this one off allocation
    Util util;

    file.open(fileName);

    if(!file.is_open()) {
        return;
    }

    uint8_t byte;
    while (file >> std::hex >> byte) {
        bytes.push_back(byte);
    }

    uint16_t programLocation = 0xC000;
    for(int i = 16; i < bytes.size(); i++) {
        byte = bytes[i];
        cpu->memory[programLocation++] = byte;
    }

    file.close();
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
//    CPUTest cpuTest;
//    cpuTest.runAllTests();

    CPU *cpu = new CPU();
    cpu->debug = true;

    Assembler assembler(cpu);
    string fileName = "sample_programs/testSubroutines2.asm";
    assembler.readFile(fileName.c_str());

//    //TODO: VERY UNFINISHED
//    string fileName = "sample_programs/nestest.nes";
//    readBinaryFile(cpu, fileName);

    printMemory(0x0600, 0x6ff, cpu);
//    printMemory(0x0000, 0x0FFFF, cpu);
//
//    cpu->memory[0x0000] = 0xa9;
//    cpu->memory[0x0001] = 0x02;
//    cpu->memory[0x0002] = 0x8d;
//    cpu->memory[0x0003] = 0xfe;
//    cpu->memory[0x0004] = 0xff;
//    cpu->memory[0x0005] = 0xa9;
//    cpu->memory[0x0006] = 0x03;
//    cpu->memory[0x0007] = 0x8d;
//    cpu->memory[0x0008] = 0xff;
//    cpu->memory[0x0009] = 0xff;
//    cpu->memory[0x000a] = 0x00;
//
//    cpu->memory[0x0302] = 0xa9;
//    cpu->memory[0x0303] = 0x01;
//    cpu->memory[0x0304] = 0xaa;
//    cpu->memory[0x0305] = 0x40;

    bool debug = true;
    cpu->programCounter = cpu->programStart;
//    cpu->programCounter = 0x0000;
    while(true) {

        if(debug) {
            //this forces the user to press enter to step through the code
            cin.ignore();

//            printDebugInformation(cpu);
        }

        cpu->execute();
//        i++;
    }

    cpu->programCounter++;

    //print one last time to see the results of the last instruction
//    cout << endl;
//    printDebugInformation(cpu);
//    cout << endl << endl;

    cout << endl;
    cout << "Final PC: " << hex << setw(4) << cpu->programCounter << endl << endl;
    printMemory(0x0500, 0x05FF, cpu);
    cout << endl;
    printStack(cpu);


    return 0;
}
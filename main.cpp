#include <iostream>
#include "emulator/Assembler.h"
#include "emulator/Util.h"
#include "emulator/CPU.h"
#include "test_emulator/cpu_tests/CPUTest.h"

using namespace std;

void printMemory(uint16_t start, uint16_t end, CPU* cpu) {
    int x = 0;

    //printf("%04x:  ", start);
    for(int i = start; i <= end; i++) {
        printf("%02x  ", cpu->memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
            //printf("%04x:  ", i);
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
    printf("%d %d %d %d %d %d %d %d\n", cpu->flags.negative, cpu->flags.overflow, cpu->flags.ignored, cpu->flags.breakFlag, cpu->flags.decimal, cpu->flags.interrupt, cpu->flags.zero, cpu->flags.carry);

    cout << "Stack: " << endl;
    printStack(cpu);
    cout << endl;

    cout << "Next instruction results to be printed: " << endl << endl;
}

int main() {
    //in hindsight, adding google testing wasn't really a good idea for me
//    CPUTest cpuTest;
//    cpuTest.runAllTests();

    CPU *cpu = new CPU();

    Assembler assembler(cpu);
    string fileName = "sample_programs/testSBC.asm";
    assembler.readFile(fileName.c_str());

    //TODO: VERY UNFINISHED
//    string fileName = "sample_programs/nestest.nes";
//    readBinaryFile(cpu, fileName);

    printMemory(0x0600, 0x0fff, cpu);

    bool debug = true;
    cpu->programCounter = cpu->programStart;
    while(cpu->memory[cpu->programCounter] != 0) {

        if(debug) {
            //this forces the user to press enter to step through the code
            cin.ignore();

//            printDebugInformation(cpu);
        }

        cpu->execute();
    }

    cpu->programCounter++;

    //print one last time to see the results of the last instruction
//    cout << endl;
//    printDebugInformation(cpu);
//    cout << endl << endl;

    printMemory(0x0000, 0x02FF, cpu);

    return 0;
}
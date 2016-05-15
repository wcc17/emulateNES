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
    //printStack(cpu);
    cout << endl;

    cout << "Next instruction results to be printed: " << endl << endl;
}

int main() {
    //in hindsight, adding google testing wasn't really a good idea
//    CPUTest cpuTest;
//    cpuTest.runAllTests();

    CPU *cpu = new CPU();
    Assembler assembler(cpu);

    string fileName = "sample_programs/testROR.asm";
    assembler.readFile(fileName.c_str());

    printMemory(0x0600, 0x0700, cpu);

    bool debug = true;
    cpu->programCounter = cpu->programStart;
    while(cpu->memory[cpu->programCounter] != 0) {

        if(debug) {
            //this forces the user to press enter to step through the code
            cin.ignore();

            printDebugInformation(cpu);
        }

        cpu->executeOpCode();
    }

    cpu->programCounter++;

    //print one last time to see the results of the last instruction
    cout << endl;
    printDebugInformation(cpu);
    cout << endl << endl;

    printMemory(0x0000, 0x02FF, cpu);

    return 0;
}
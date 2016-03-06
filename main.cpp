#include <iostream>
#include "Assembler.h"
#include "Util.h"
#include "CPU.h"

using namespace std;

int main() {
    CPU *cpu = new CPU();
    Assembler assembler(cpu);

    string fileName = "firstprogram.asm";
    assembler.readFile(fileName.c_str());

    bool debug = true;

    cpu->programCounter = cpu->programStart;
    while(cpu->memory[cpu->programCounter] != 0) {

        if(debug) {
            //this forces the user to press enter to step through the code
            cin.ignore();

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

            cout << "Next instruction results to be printed: " << endl;
        }

        cpu->executeOpCode();
    }

    return 0;
}
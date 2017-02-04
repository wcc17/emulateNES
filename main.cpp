#include "Emulator.h"
//#include "emulator/Assembler.h"

int main() {
    //    Assembler assembler(cpu);
//    string fileName = "sample_programs/testSubroutines2.asm";
//    assembler.readFile(fileName.c_str());

    Emulator emulator;
    return emulator.onExecute();
}
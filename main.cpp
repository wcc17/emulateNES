#include <NES.h>
//#include "emulator/Assembler.h"
//#include "Emulator.h"

using namespace std;

int main() {
    //    Assembler assembler(cpu);
//    string fileName = "sample_programs/testSubroutines2.asm";
//    assembler.readFile(fileName.c_str());

    //set up NES
    NES *nes = new NES();
    nes->start();

    return 0;

//    Emulator emulator;
//
//    return emulator.onExecute();
}
//
// Created by Christian Curry on 2/15/16.
//

#ifndef EMULATE6502_ASSEMBLER_H
#define EMULATE6502_ASSEMBLER_H

using namespace std;
#include <string>
#include <fstream>
#include "opcodes.h"
#include "addressingmodes.h"
#include "CPU.h"


class Assembler {

    //global variables
    //int x;
    CPU* cpu;

public:
    Assembler(CPU* cpu);
    void readFile(string filename);

private:
    void storeProgramInMemory(string instruction, string argument, uint16_t &programLocation);
    bool isArgument(string word);
    bool isLabel(string word);
    uint16_t convertStringToWord(string argument);
    uint8_t getLowByte(uint16_t word);
    uint8_t getHighByte(uint16_t word);

protected:

};


#endif //EMULATE6502_ASSEMBLER_H

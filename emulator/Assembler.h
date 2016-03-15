//
// Created by Christian Curry on 2/15/16.
//

#ifndef EMULATE6502_ASSEMBLER_H
#define EMULATE6502_ASSEMBLER_H

using namespace std;
#include <string>
#include <fstream>
#include <map>
#include "opcodes.h"
#include "addressingmodes.h"
#include "CPU.h"


class Assembler {

    //global variables
    //int x;
    CPU* cpu;
    std::map <string, uint16_t> labels;

public:
    Assembler(CPU* cpu);
    void readFile(string filename);

private:
    void storeProgramInMemory(string instruction, string argument, uint16_t &programLocation);
    AddressingMode determineAddressingMode(string argument);
    string trimArgument(string argument);
    bool isArgument(string word);

    //this method checks to see if the string being passed is a label being declared for the first time.
    bool isLabel(string word);

    //this method checks to see if the string passed in is a label thats already declared and exists in labels.
    //will return the memory location that the label points to
    bool isExistingLabel(string word);

    uint16_t convertStringToWord(string argument);
    uint8_t getLowByte(uint16_t word);
    uint8_t getHighByte(uint16_t word);

protected:

};


#endif //EMULATE6502_ASSEMBLER_H

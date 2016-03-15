//
// Created by Christian Curry on 2/15/16.
//

#ifndef EMULATE6502_ASSEMBLER_H
#define EMULATE6502_ASSEMBLER_H

using namespace std;
#include <string>
#include <fstream>
#include <map>
#include <vector>
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

    /**goes through program and retrieves all labels and their memory locations before actually assembling program
    this is basically the same as what happens in readFile, but we're not storing anything in the 6502 memory yet **/
    void storeLabels(vector<string> words);
    uint16_t determineInstructionLocation(string instruction, string argument, uint16_t programLocation);

    //this method checks to see if the string being passed is a label being declared for the first time.
    bool isLabel(string word);

    /**this method checks to see if the string passed in is a label thats already declared and exists in labels.
    will return the memory location that the label points to **/
    bool isExistingLabel(string word);

    /** since relative addressing and zeropage is so similar when reading from file, we need to see if the instruction is relative
    if so, the addressing mode will be relative. Even though zeropage and relative behave almost exactly alike, this is better
    for clarity. **/
    bool isBranchInstruction(string instruction);

    uint16_t convertStringToWord(string argument);
    string convertWordToString(uint16_t word);
    uint8_t getLowByte(uint16_t word);
    uint8_t getHighByte(uint16_t word);

protected:

};


#endif //EMULATE6502_ASSEMBLER_H

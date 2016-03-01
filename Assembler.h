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


class Assembler {

    //global variables
    //int x;

public:
    Assembler();
    void readFile(string filename);

private:
    void storeProgramInMemory(string instruction, string argument);
    void executeOpCode(); //TODO: THIS SHOULD PROBLY BE IN ANOTHER CLASS
    bool isArgument(string word);
    bool Assembler::isLabel(string word);

protected:

};


#endif //EMULATE6502_ASSEMBLER_H

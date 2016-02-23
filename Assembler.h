//
// Created by Christian Curry on 2/15/16.
//

#ifndef EMULATE6502_ASSEMBLER_H
#define EMULATE6502_ASSEMBLER_H

using namespace std;
#include <string>
#include <fstream>


class Assembler {

    //global variables
    //int x;

public:
    Assembler();
    void readFile(string filename);

private:
    void getMachineCode(string instruction, string argument);
    bool isArgument(string word);

protected:

};


#endif //EMULATE6502_ASSEMBLER_H

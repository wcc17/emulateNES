//
// Created by Christian Curry on 2/15/16.
//

#include "Assembler.h"
#include <iostream>
#include <vector>

using namespace std;

Assembler::Assembler() {
    //init
}

/**
 * Some rules:
 *
 *  immediate addressing mode = indicated by # before operand
 *  absolute and zero page = indicated by $ before the operand
 *  implied = no operand is given
 *  accumulator = no operand is given
 *  indexed and zero page indexed = indicated by $ before the operand
 *  indirect = operand begins with parenthesis
 *  pre indexed indirect = parenthesis
 *
 *  so operands can start with #, $, or (
 */

void Assembler::readFile(string fileName) {
    ifstream file;
    vector<string> words;   //just going to let words vector increase by itself rather tahn trying to reserve space for this one off allocation

    file.open(fileName);

    if(!file.is_open()) {
        return;
    }

    string word;
    while (file >> word) {
        words.push_back(word);
    }

    string instruction;
    string argument;
    for(int i = 0; i < words.size(); i++) {
        instruction = words[i++];
        argument = words[i];

        if(isArgument(argument)) {
            storeProgramInMemory(instruction, argument);
        } else {
            i -= 1;
            storeProgramInMemory(instruction, NULL);
        }
    }

    file.close();
}

bool Assembler::isArgument(string word) {
    char w = word.at(0);

    if(w == '#'
            || w == '$'
            || w == '(') {
        return true;
    } else {
        return false;
    }
}

//TODO: this is technically dissassembly. this should be in a differenct class, along with readfile
//NOTE: WHEN WE'RE READING A ROM FILE, JUST STORE THE BYTES FROM THE FILE IN PROGRAM MEMORY 1 BY 1
//here we take the instructions from an assembly file (one that uses the syntax, LDA $whatever)
//convert the string to byte
//store that byte in program memory
//then we can execute the machine code from there
void Assembler::storeProgramInMemory(string instruction, string argument) {
    //get the addressing mode from the argument first
    //set up constants for addressing modes and switch addressing mode
    //inside those cases, switch the instructions that the string instruction could be

    instruction = "LDA";

    if(instruction == "LDA") {
        cout << "lda" << endl;
    } else {
        cout << "default" << endl;
    }
}

//TODO: THIS SHOULD PROBLY BE IN ANOTHER CLASS
//this will take the op codes from program memory and execute them one at a time
void Assembler::executeOpCode() {

    //if im passing in straight machine code
    uint8_t opcode = 0xA9;

    switch(opcode) {
        case LDA_IMM:
            cout << "lda imm" << endl;
            break;
        default:
            cout << "default" << endl;
            break;
    }

}


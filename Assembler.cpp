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
    size_t fileSize;

    file.open(fileName);

    if(!file.is_open()) {
        return;
    }

    fileSize = getFileSize(file);

    vector<string> words;
    words.reserve(fileSize);

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
            getMachineCode(instruction, argument);
        } else {
            i -= 1;
            getMachineCode(instruction, NULL);
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

size_t Assembler::getFileSize(ifstream &file) {
    streampos fileSize = file.tellg();
    file.seekg(0, std::ios::end);
    size_t fS = file.tellg() - fileSize;

    return fS;
}

void Assembler::getMachineCode(string instruction, string argument) {
    //if instruction starts with #, $, ( then its an argument
    //otherwise, its an instruction
    cout << instruction << " " << argument << endl;
}


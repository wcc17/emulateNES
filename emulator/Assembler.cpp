//
// Created by Christian Curry on 2/15/16.
//

#include "Assembler.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

Assembler::Assembler(CPU* c) {
    cpu = c;
}

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
    uint16_t programLocation = cpu->programStart;
    for(int i = 0; i < words.size(); i++) {
        instruction = words[i++];
        argument = words[i];

        if(isArgument(argument)) {
            storeProgramInMemory(instruction, argument, programLocation);
        } else {
            i -= 1;
            storeProgramInMemory(instruction, NULL, programLocation);
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

bool Assembler::isLabel(string word) {
    //check to see if last char in string is the : symbol
    //if so, this is a label, needs to be handled accordingly
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

//NOTE: WHEN WE'RE READING A ROM FILE, JUST STORE THE BYTES FROM THE FILE IN PROGRAM MEMORY 1 BY 1
//here we take the instructions from an assembly file (one that uses the syntax, LDA $whatever)
//convert the string to byte
//store that byte in program memory
//then we can execute the machine code from there
void Assembler::storeProgramInMemory(string instruction, string argument, uint16_t &programLocation) {
    //get the addressing mode from the argument first
    //set up constants for addressing modes and switch addressing mode
    //inside those cases, switch the instructions that the string instruction could be

    //LSR A is a valid instruction/argument combo. A means that lSR is doing something to the accumulator. Called Accumulator
    //if the first symbol is a # the argument is a value. IMMEDIATE mode
    //if there is no argument, it is implied addressing mode
    //relative addressng will look like a label without the : at the end
    //if the first symbol is a $ the argument is a memory location. These will be two bytes
    //if the first symbol is a $ and its one byte, its zero page addressing, the byte is a memory location offset
    //indirect starts with a parenthesis
    //can have $XX, Y or $XX, X ( i think ) for absolute indexed. where XX is two bytes
    //zero page indexed is $XX, X where XX is only one byte
    //indexed indirect ex. LDA ($20,X) ---> need to read more about this
    //indirect indexed ex. LDA ($86),Y ---> need to read mroe about this

    //initializing this with null so that it doesn't initialize to the first value in addressingmodes enum
    //this is so i can pick it up later if for some reason we don't have an addressing mode
    AddressingMode addressingMode = NULL_ADDRESSING_MODE;

    if(argument[0] == '#') {
        //erase # and $ from beginning of argument
        argument.erase(0, 2);

        addressingMode = IMMEDIATE;
    } else if(argument[0] == '$') {

        //erase the first $ character
        argument.erase(0, 1);

        //if theres a comma in the argument, its zero page. otherwise, check if the argument is 4 digits or 2 digits
        if(argument.find(",") != string::npos) {

            //convert all characters to lower case for simplicity because x and y in zeropage indexed instructions aren't case sensitive
            for(int i = 0; i < argument.size(); i++) {
                argument[i] = tolower(argument[i]);
            }

            if(argument.find("x") != string::npos ) {
                addressingMode = ZERO_PAGEX;
            } else if(argument.find("y") != string::npos) {
                addressingMode = ZERO_PAGEY;
            }

        } else if(argument.size() == 4) {
            //TODO: CHECKING THE SIZE CANNOT BE THE ONLY CHECK HERE. IF THERE ARE SPACES IN THE ARGUMENT THIS WILL FAIL
            //TODO: SHOULD BE CHECKING FOR FOUR OR TWO DIGITS, NOT JUST THE LENGTH OF THE STRING
            //here argument will either be xxxx or xx,x
            //both are 4 characters
            addressingMode = ABSOLUTE;

        } else if(argument.size() == 2) {
            addressingMode = ZERO_PAGE;
        }
    } else {
        addressingMode = IMPLIED;
    }

    uint16_t arg = convertStringToWord(argument);

    switch(addressingMode) {
        case IMPLIED:
            if(instruction == "INX") {
                cpu->storeByteInMemory(TAX, programLocation++);
            } else if(instruction == "TAX") {
                cpu->storeByteInMemory(TAX, programLocation++);
            }
            break;
        case IMMEDIATE:
            if(instruction == "ADC") {
                cpu->storeByteInMemory(ADC_IMMEDIATE, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            } else if(instruction == "LDA") {
                //store LDA_IMM in memory
                cpu->storeByteInMemory(LDA_IMMEDIATE, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            }
            break;
        case ZERO_PAGE:
            if(instruction == "ADC") {
                cpu->storeByteInMemory(ADC_ZEROPAGE, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            } else if (instruction == "STA") {
                cpu->storeByteInMemory(STA_ZEROPAGE, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            }
            break;
        case ZERO_PAGEX:
            if(instruction == "ADC") {
                cpu->storeByteInMemory(ADC_ZEROPAGEX, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            } else if (instruction == "STA") {
                cpu->storeByteInMemory(STA_ZEROPAGEX, programLocation++);
                cpu->storeByteInMemory(getLowByte(arg), programLocation++):
            }
            break;
        case ZERO_PAGEY:
            break;
        case ABSOLUTE :
            if(instruction == "LDA") {
                cpu->storeByteInMemory(LDA_ABSOLUTE, programLocation++);
                cpu->storeWordInMemory(getLowByte(arg), getHighByte(arg), programLocation += 2);
                programLocation += 2; //because we move two spots in storeWordInMemory
            } else if(instruction == "STA") {
                //store STA_ABS in memory
                cpu->storeByteInMemory(STA_ABSOLUTE, programLocation++);
                cpu->storeWordInMemory(getLowByte(arg), getHighByte(arg), programLocation);
                programLocation += 2; //because we move two spots in storeWordInMemory
            }
            break;
        case ABSOLUTEX:
            break;
        case ABSOLUTEY:
            break;
        case INDIRECTX:
            break;
        case INDIRECTY:
            break;
    }
}

//TODO: MOVE THESE TO UTIL. THEY COULD BE USEFUL ELSEWHERE.
uint16_t Assembler::convertStringToWord(string argument) {
    uint16_t x;

    stringstream ss;
    ss << hex << argument;
    ss >> x;

    return x;
}


//You can cast it to kill the upper-byte of the 16-bit variable:
//uint16_t A = 120;
//uint8_t B;
//
//B = (uint8_t)A; // Get lower byte of 16-bit var
//If you need the upper byte, shift and cast instead:
//uint16_t A = 120;
//uint8_t B;
//
//B = (uint8_t)(A >> 8); // Get upper byte of 16-bit var
uint8_t Assembler::getLowByte(uint16_t word) {
    uint8_t byte;

    byte = (uint8_t)word;

    return byte;
}

uint8_t Assembler::getHighByte(uint16_t word) {
    uint8_t byte;

    byte = (uint8_t)(word >> 8);

    return byte;
}




//
// Created by Christian Curry on 2/15/16.
//

#include "Assembler.h"
#include <iostream>
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

    storeLabels(words);

    string instruction;
    string argument;
    uint16_t programLocation = cpu->programStart;
    for(int i = 0; i < words.size(); i++) {

        instruction = words[i++];
        if(i != words.size()) {
            argument = words[i];
        } else {
            //if we're at the end of the program, we don't want to pick up something weird here
            argument = "";
        }


        //different scenarios so far
        //1. instruction is a normal instruction and argument is a normal argument
        //2. instruction is implied, no argument will exist
        //3. instruction can be a label. this will happen if the last instruction had a valid argument
        //4. argument can be a label. this will happen if the last insturction was implied OR
        //   if an instruction is using the label as an argument


        //if label is not an instruction
        if(isLabel(instruction)) {
            //this is already taken care of in storeLabels
//            instruction.erase(instruction.length() - 1);
//            labels[instruction] = programLocation;

            if(i != words.size()) {
                //if we're already at the end of the program, don't go back
                i--;
            }
        } else if(isArgument(argument)) {
            //STORE THE NORMAL INSTRUCTION+ARGUMENT AFTER THE INSTRUCTION
            storeProgramInMemory(instruction, argument, programLocation);
        } else if(isExistingLabel(argument)) {
            //CHECK TO SEE IF ARGUMENT CORRESPONDS TO A VALID LABEL
            //THAT HAS ALREADY BEEN DECLARED EARLIER
            //memoryLocation will be the location of the original label
            //need the difference between the programCounter now and that memroyLocation
            //if negative, store as twos complement. the instructions will take care of th conversion from here
            uint16_t labelMemoryLocation = labels[argument];

            //plus two because we want the position after this instruction + argument combo
            //using 8 bit offset first so that negative numbers are converted correctly
            uint8_t offset = labelMemoryLocation - (programLocation + 2);
            uint16_t wordOffset = offset;
            argument = convertWordToString(wordOffset);
            storeProgramInMemory(instruction, argument, programLocation);
        }  else {
            //IF NOTHING ELSE, THE INSTRUCTION IS IMPLIED. STORE IT LIKE THIS
            if(i != words.size()) {
                //if we're already at the end of the program, don't go back
                i -= 1;
            }
            storeProgramInMemory(instruction, "", programLocation);
        }
    }

    file.close();
}

void Assembler::storeLabels(vector<string> words) {
    uint16_t programLocation = cpu->programStart;
    string instruction;
    string argument;
    for(int i = 0; i < words.size(); i++) {
        instruction = words[i++];
        if(i != words.size()) {
            argument = words[i];
        } else {
            argument = "";
        }

        if(isLabel(instruction)) {
            instruction.erase(instruction.length() - 1);
            labels[instruction] = programLocation;

            if(i != words.size()) {
                i--;
            }
        } else if(isArgument(argument)) {
            programLocation = determineInstructionLocation(instruction, argument, programLocation);
        } else if(isExistingLabel(argument)) {
            uint16_t memoryLocation = labels[argument];
            programLocation += 2; //a label will always be one byte. the instruction willl be one byte
        }  else {
            if(i != words.size()) {
                i -= 1;
            }
            programLocation = determineInstructionLocation(instruction, "", programLocation);
        }
    }
}

uint16_t Assembler::determineInstructionLocation(string instruction, string argument, uint16_t programLocation) {
    AddressingMode addressingMode = determineAddressingMode(argument);

    switch(addressingMode) {
        case NULL_ADDRESSING_MODE:
            //FAIL
            break;
        case ACCUMULATOR:
            programLocation++;
            break;
        case IMPLIED:
            programLocation++;
            break;
        case IMMEDIATE:
            programLocation += 2;
            break;
        case ZERO_PAGE:
            programLocation += 2;
            break;
        case ZERO_PAGEX:
            programLocation += 2;
            break;
        case ZERO_PAGEY:
            programLocation += 2;
            break;
        case ABSOLUTE:
            programLocation += 3;
            break;
        case ABSOLUTEX:
            programLocation += 3;
            break;
        case ABSOLUTEY:
            programLocation += 3;
            break;
        case INDEXED_INDIRECTX:
            programLocation += 2;
            break;
        case INDIRECT_INDEXEDY:
            programLocation += 2;
            break;
    }

    return programLocation;
}

AddressingMode Assembler::determineAddressingMode(string argument) {
    AddressingMode addressingMode = NULL_ADDRESSING_MODE;

    if(argument[0] == 'A') {
        addressingMode = ACCUMULATOR;
    }
    else if(argument[0] == '#') {
        //erase # and $ from beginning of argument
        argument.erase(0, 2);

        addressingMode = IMMEDIATE;
    } else if(argument[0] == '$') {

        //erase the first $ character
        argument.erase(0, 1);

        int numHexDigits = 0;
        for(int i = 0; i < argument.size(); i++) {
            if(isxdigit(argument[i])) {
                numHexDigits++;
            }
        }

        //if theres a comma in the argument, its zero page. otherwise, check if the argument is 4 digits or 2 digits
        if(argument.find(",") != string::npos) {

            //convert all characters to lower case for simplicity because x and y in zeropage indexed instructions aren't case sensitive
            for(int i = 0; i < argument.size(); i++) {
                argument[i] = tolower(argument[i]);
            }

            if(argument.find("x") != string::npos ) {
                if(numHexDigits == 4) {
                    addressingMode = ABSOLUTEX;
                } else if(numHexDigits == 2) {
                    addressingMode = ZERO_PAGEX;
                }
            } else if(argument.find("y") != string::npos) {
                if(numHexDigits == 4) {
                    addressingMode = ABSOLUTEY;
                } else if(numHexDigits == 2) {
                    addressingMode = ZERO_PAGEY;
                }
            }

        } else if(argument.size() == 4) {
            //TODO: CHECKING THE SIZE CANNOT BE THE ONLY CHECK HERE. IF THERE ARE SPACES IN THE ARGUMENT THIS WILL FAIL
            //TODO: SHOULD BE CHECKING FOR FOUR OR TWO DIGITS, NOT JUST THE LENGTH OF THE STRING
            addressingMode = ABSOLUTE;

        } else if(numHexDigits == 2) {
            addressingMode = ZERO_PAGE;
        }
    } else if(argument[0] == '(') {

        //LDA ($20,X)
        //LDA ($86),Y

        argument.erase(0, 2);

        //convert all characters to lower case for simplicity because x and y in zeropage indexed instructions aren't case sensitive
        for(int i = 0; i < argument.size(); i++) {
            argument[i] = tolower(argument[i]);
        }

        //TODO: THIS IS NOT THE FINAL SOLUTION. ASSEMBLER IS LOW PRIORITY FOR NOW
        if(argument[2] ==  ',') {
            addressingMode = INDEXED_INDIRECTX;
        } else if(argument[2] == ')') {
            addressingMode = INDIRECT_INDEXEDY;
        }

    } else {
        addressingMode = IMPLIED;
    }

    return addressingMode;
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

    if(isBranchInstruction(instruction)) {
        addressingMode = RELATIVE;
    } else {
        addressingMode = determineAddressingMode(argument);
    }
    argument = trimArgument(argument);

    uint16_t arg = convertStringToWord(argument);

    OpCodes opcode = BRK;
    switch(addressingMode) {
        case NULL_ADDRESSING_MODE:
            opcode = BRK;
            //fail
            break;
        case ACCUMULATOR:
            if(instruction == "ASL") {
                opcode = ASL_ACCUMULATOR;
            }
            cpu->storeByteInMemory(opcode, programLocation++);
            break;
        case RELATIVE:
            if(instruction == "BPL") {
                opcode = BPL;
            } else if(instruction == "BMI") {
                opcode = BMI;
            } else if(instruction == "BVC") {
                opcode = BVC;
            } else if(instruction == "BVS") {
                opcode = BVS;
            } else if(instruction == "BCC") {
                opcode = BCC;
            } else if(instruction == "BCS") {
                opcode = BCS;
            } else if(instruction == "BNE") {
                opcode = BNE;
            } else if(instruction == "BEQ") {
                opcode = BEQ;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            break;
        case IMPLIED:
            if(instruction == "DEX") {
                opcode = DEX;
            } else if(instruction == "INX") {
                opcode = INX;
            } else if(instruction == "TAX") {
                opcode = TAX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);

            break;
        case IMMEDIATE:
            if(instruction == "ADC") {
                opcode = ADC_IMMEDIATE;
            } else if(instruction == "AND") {
                opcode = AND_IMMEDIATE;
            } else if(instruction == "CPX") {
                opcode = CPX_IMMEDIATE;
            } else if(instruction == "LDA") {
                opcode = LDA_IMMEDIATE;
            } else if(instruction == "LDX") {
                opcode = LDX_IMMEDIATE;
            } else if(instruction == "LDY") {
                opcode = LDY_IMMEDIATE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);

            break;
        case ZERO_PAGE:
            if(instruction == "ADC") {
                opcode = ADC_ZEROPAGE;
            } else if(instruction == "AND") {
                opcode = AND_ZEROPAGE;
            } else if(instruction == "ASL") {
                opcode = ASL_ZEROPAGE;
            } else if(instruction == "BIT") {
                opcode = BIT_ZEROPAGE;
            } else if(instruction == "LDA") {
                opcode = LDA_ZEROPAGE;
            } else if (instruction == "LDX") {
                opcode = LDX_ZEROPAGE;
            } else if (instruction == "LDY") {
                opcode = LDY_ZEROPAGE;
            } else if (instruction == "STA") {
                opcode = STA_ZEROPAGE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);

            break;
        case ZERO_PAGEX:
            if(instruction == "ADC") {
                opcode = ADC_ZEROPAGEX;
            } else if(instruction == "AND") {
                opcode = AND_ZEROPAGEX;
            } else if(instruction == "ASL") {
                opcode = ASL_ZEROPAGEX;
            } else if(instruction == "LDA") {
                opcode = LDA_ZEROPAGEX;
            } else if(instruction == "LDY") {
                opcode = LDY_ZEROPAGEX;
            } else if(instruction == "STA") {
                opcode = STA_ZEROPAGEX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);

            break;
        case ZERO_PAGEY:
            if(instruction == "LDX") {
                opcode = LDX_ZEROPAGEY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            break;
        case ABSOLUTE :
            if(instruction == "ADC") {
                opcode = ADC_ABSOLUTE;
            } else if(instruction == "AND") {
                opcode = AND_ABSOLUTE;
            } else if(instruction == "ASL") {
                opcode = ASL_ABSOLUTE;
            } else if(instruction == "BIT") {
                opcode = BIT_ABSOLUTE;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTE;
            } else if(instruction == "LDX") {
                opcode = LDX_ABSOLUTE;
            } else if(instruction == "LDY") {
                opcode = LDY_ABSOLUTE;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTE;
            } else if(instruction == "STX") {
                opcode = STX_ABSOLUTE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(getLowByte(arg), getHighByte(arg), programLocation);
            programLocation += 2; //because we move two spots in storeWordInMemory

            break;
        case ABSOLUTEX:
            if(instruction == "ADC") {
                opcode = ADC_ABSOLUTEX;
            } else if(instruction == "AND") {
                opcode = AND_ABSOLUTEX;
            } else if(instruction == "ASL") {
                opcode = ASL_ABSOLUTEX;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTEX;
            } else if(instruction == "LDY") {
                opcode = LDY_ABSOLUTEX;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTEX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(getLowByte(arg), getHighByte(arg), programLocation);
            programLocation += 2;
            break;
        case ABSOLUTEY:
            if(instruction == "ADC") {
                opcode = ADC_ABSOLUTEY;
            } else if(instruction == "AND") {
                opcode = AND_ABSOLUTEY;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTEY;
            } else if(instruction == "LDX") {
                opcode = LDX_ABSOLUTEY;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTEY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(getLowByte(arg), getHighByte(arg), programLocation);
            programLocation += 2;
            break;
        case INDEXED_INDIRECTX:
            if(instruction == "ADC") {
                opcode = ADC_INDEXED_INDIRECTX;
            } else if(instruction == "AND") {
                opcode = AND_INDEXED_INDIRECTX;
            } else if(instruction == "LDA") {
                opcode = LDA_INDEXED_INDIRECTX;
            } else if(instruction == "STA") {
                opcode = STA_INDEXED_INDIRECTX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            break;
        case INDIRECT_INDEXEDY:
            if(instruction == "ADC") {
                opcode = ADC_INDIRECT_INDEXEDY;
            } else if(instruction == "AND") {
                opcode = AND_INDIRECT_INDEXEDY;
            } else if(instruction == "LDA") {
                opcode = LDA_INDIRECT_INDEXEDY;
            } else if(instruction == "STA") {
                opcode = STA_INDIRECT_INDEXEDY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(getLowByte(arg), programLocation++);
            break;
    }
}

bool Assembler::isArgument(string word) {
    if(word != "") {
        char w = word.at(0);

        if( (w == 'A' && word.length() <= 1)
            || w == '#'
            || w == '$'
            || w == '(') {
            return true;
        }
    }

    return false;
}

bool Assembler::isLabel(string word) {
    if(word != "") {
        char w = word.back();

        if(w == ':') {
            return true;
        }
    }

    return false;
}

bool Assembler::isExistingLabel(string word) {
    if(word != "") {
        if(labels.find(word) != labels.end()) {
            return true;
        }
    }

    return false;
}

bool Assembler::isBranchInstruction(string instruction) {
    if(instruction == "BPL") {
        return true;
    } else if(instruction == "BMI") {
        return true;
    } else if(instruction == "BVC") {
        return true;
    } else if(instruction == "BVS") {
        return true;
    } else if(instruction == "BCC") {
        return true;
    } else if(instruction == "BCS") {
        return true;
    } else if(instruction == "BNE") {
        return true;
    } else if(instruction == "BEQ") {
        return true;
    }

    return false;
}

//trims argument string down to only the memory location or value it contains
string Assembler::trimArgument(string argument) {
    if (argument[0] == '#') {
        //erase # and $ from beginning of argument
        argument.erase(0, 2);
    } else if (argument[0] == '$') {

        //erase the first $ character
        argument.erase(0, 1);
    } else if (argument[0] == '(') {
        argument.erase(0, 2);
        argument.erase(4, argument.length() - 1);
    }

    return argument;
}

//TODO: MOVE THESE TO UTIL. THEY COULD BE USEFUL ELSEWHERE.
uint16_t Assembler::convertStringToWord(string argument) {
    uint16_t x;

    stringstream ss;
    ss << setfill('0') << setw(4) << hex << argument;
    ss >> x;

    return x;
}

string Assembler::convertWordToString(uint16_t word) {
    string s;

    stringstream ss;
    ss << hex << word;
    s = ss.str();

    return s;
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




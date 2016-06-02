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

            //only need offset
            if(isBranchInstruction(instruction)) {
                //need the difference between the programCounter now and that memroyLocation
                //if negative, store as twos complement. the instructions will take care of th conversion from here
                uint16_t labelMemoryLocation = labels[argument];

                //plus two because we want the position after this instruction + argument combo
                //using 8 bit offset first so that negative numbers are converted correctly
                uint8_t offset = labelMemoryLocation - (programLocation + 2);
                uint16_t wordOffset = offset;
                argument = util.convertWordToString(wordOffset);
                storeProgramInMemory(instruction, argument, programLocation);
            } else {
                //need entire address of existing label
                uint16_t labelMemoryLocation = labels[argument];
                argument = util.convertWordToString(labelMemoryLocation);

                while (argument.length() < 4) {
                    argument.insert(0, "0");
                }
                argument.insert(0, "$");

                storeProgramInMemory(instruction, argument, programLocation);
            }
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
        } else if(!isInstruction(argument) && argument != "") {
            /**if isInstruction returns false for an argument, that means the argument is a label that hasn't actually been indexed yet. **/
            //branch instructions use a relative value for the labels. other instructions will use an absolute value for the label
            if(isBranchInstruction(instruction)) {
                programLocation += 2;
            } else {
                programLocation += 3;
            }
        }  else {
            if(i != words.size()) {
                i -= 1;
            }
            programLocation = determineInstructionLocation(instruction, "", programLocation);
        }
    }
}

//THIS IS STRICTLY FOR INDEXING LABELS
uint16_t Assembler::determineInstructionLocation(string instruction, string argument, uint16_t programLocation) {
    AddressingMode addressingMode = determineAddressingMode(argument);

    //compiler generates a warning that RELATIVE is not used here. This is okay, relative is handled else where
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
        case INDIRECT:
            programLocation += 3;
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
        //JMP ($2222) -> indirect

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
        } else {
            addressingMode = INDIRECT;
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

    uint16_t arg = util.convertStringToWord(argument);

    OpCodes opcode = BRK;
    switch(addressingMode) {
        case NULL_ADDRESSING_MODE:
            opcode = BRK;
            //fail
            break;
        case ACCUMULATOR:
            if (instruction == "ASL") {
                opcode = ASL_ACCUMULATOR;
            } else if (instruction == "LSR") {
                opcode = LSR_ACCUMULATOR;
            } else if (instruction == "ROL") {
                opcode = ROL_ACCUMULATOR;
            } else if(instruction == "ROR") {
                opcode = ROR_ACCUMULATOR;
            }
            cpu->storeByteInMemory(opcode, programLocation++);
            break;
        case RELATIVE:
            if (instruction == "BPL") {
                opcode = BPL;
            } else if (instruction == "BMI") {
                opcode = BMI;
            } else if (instruction == "BVC") {
                opcode = BVC;
            } else if (instruction == "BVS") {
                opcode = BVS;
            } else if (instruction == "BCC") {
                opcode = BCC;
            } else if (instruction == "BCS") {
                opcode = BCS;
            } else if (instruction == "BNE") {
                opcode = BNE;
            } else if (instruction == "BEQ") {
                opcode = BEQ;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);
            break;
        case IMPLIED:
            if(instruction == "CLC") {
                opcode = CLC;
            } else if(instruction == "CLD") {
                opcode = CLD;
            } else if(instruction == "CLI") {
                opcode = CLI;
            } else if(instruction == "CLV") {
                opcode = CLV;
            } else if(instruction == "DEX") {
                opcode = DEX;
            } else if(instruction == "DEY") {
                opcode = DEY;
            } else if(instruction == "INX") {
                opcode = INX;
            } else if(instruction == "INY") {
                opcode = INY;
            } else if(instruction == "NOP") {
                opcode = NOP;
            } else if(instruction == "PHA") {
                opcode = PHA;
            } else if(instruction == "PHP") {
                opcode = PHP;
            } else if(instruction == "PLA") {
                opcode = PLA;
            } else if(instruction == "PLP") {
                opcode = PLP;
            } else if(instruction == "RTS") {
                opcode = RTS;
            } else if(instruction == "RTI") {
                opcode = RTI;
            } else if(instruction == "SEC") {
                opcode = SEC;
            } else if(instruction == "SED") {
                opcode = SED;
            } else if(instruction == "SEI") {
                opcode = SEI;
            } else if(instruction == "TAX") {
                opcode = TAX;
            } else if(instruction == "TAY") {
                opcode = TAY;
            } else if(instruction == "TSX") {
                opcode = TSX;
            } else if(instruction == "TXA") {
                opcode = TXA;
            } else if(instruction == "TXS") {
                opcode = TXS;
            } else if(instruction == "TYA") {
                opcode = TYA;
            }

            cpu->storeByteInMemory(opcode, programLocation++);

            break;
        case IMMEDIATE:
            if(instruction == "ADC") {
                opcode = ADC_IMMEDIATE;
            } else if(instruction == "AND") {
                opcode = AND_IMMEDIATE;
            } else if(instruction == "CMP") {
                opcode = CMP_IMMEDIATE;
            } else if(instruction == "CPX") {
                opcode = CPX_IMMEDIATE;
            } else if(instruction == "CPY") {
                opcode = CPY_IMMEDIATE;
            } else if(instruction == "EOR") {
                opcode = EOR_IMMEDIATE;
            } else if(instruction == "LDA") {
                opcode = LDA_IMMEDIATE;
            } else if(instruction == "LDX") {
                opcode = LDX_IMMEDIATE;
            } else if(instruction == "LDY") {
                opcode = LDY_IMMEDIATE;
            } else if(instruction == "ORA") {
                opcode = ORA_IMMEDIATE;
            } else if(instruction == "SBC") {
                opcode = SBC_IMMEDIATE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);

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
            } else if(instruction == "CMP") {
                opcode = CMP_ZEROPAGE;
            } else if(instruction == "CPX") {
                opcode = CPX_ZEROPAGE;
            } else if(instruction == "CPY") {
                opcode = CPY_ZEROPAGE;
            } else if(instruction == "DEC") {
                opcode = DEC_ZEROPAGE;
            } else if(instruction == "EOR") {
                opcode = EOR_ZEROPAGE;
            } else if(instruction == "INC") {
                opcode = INC_ZEROPAGE;
            } else if(instruction == "LDA") {
                opcode = LDA_ZEROPAGE;
            } else if (instruction == "LDX") {
                opcode = LDX_ZEROPAGE;
            } else if (instruction == "LDY") {
                opcode = LDY_ZEROPAGE;
            } else if(instruction == "LSR") {
                opcode = LSR_ZEROPAGE;
            } else if(instruction == "ORA") {
                opcode = ORA_ZEROPAGE;
            } else if(instruction == "ROL") {
                opcode = ROL_ZEROPAGE;
            } else if(instruction == "ROR") {
                opcode = ROR_ZEROPAGE;
            } else if(instruction == "SBC") {
                opcode = SBC_ZEROPAGE;
            } else if (instruction == "STA") {
                opcode = STA_ZEROPAGE;
            } else if (instruction == "STX") {
                opcode = STX_ZEROPAGE;
            } else if (instruction == "STY") {
                opcode = STY_ZEROPAGE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);

            break;
        case ZERO_PAGEX:
            if(instruction == "ADC") {
                opcode = ADC_ZEROPAGEX;
            } else if(instruction == "AND") {
                opcode = AND_ZEROPAGEX;
            } else if(instruction == "ASL") {
                opcode = ASL_ZEROPAGEX;
            } else if(instruction == "CMP") {
                opcode = CMP_ZEROPAGEX;
            } else if(instruction == "DEC") {
                opcode = DEC_ZEROPAGEX;
            } else if(instruction == "EOR") {
                opcode = EOR_ZEROPAGEX;
            } else if(instruction == "INC") {
                opcode = INC_ZEROPAGEX;
            } else if(instruction == "LDA") {
                opcode = LDA_ZEROPAGEX;
            } else if(instruction == "LDY") {
                opcode = LDY_ZEROPAGEX;
            } else if(instruction == "LSR") {
                opcode = LSR_ZEROPAGEX;
            } else if(instruction == "ORA") {
                opcode = ORA_ZEROPAGEX;
            } else if(instruction == "ROL") {
                opcode = ROL_ZEROPAGEX;
            } else if(instruction == "ROR") {
                opcode = ROR_ZEROPAGEX;
            } else if(instruction == "SBC") {
                opcode = SBC_ZEROPAGEX;
            } else if(instruction == "STA") {
                opcode = STA_ZEROPAGEX;
            } else if(instruction == "STY") {
                opcode = STY_ZEROPAGEX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);

            break;
        case ZERO_PAGEY:
            if(instruction == "LDX") {
                opcode = LDX_ZEROPAGEY;
            } else if(instruction == "STX") {
                opcode = STX_ZEROPAGEY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);
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
            } else if(instruction == "CMP") {
                opcode = CMP_ABSOLUTE;
            } else if(instruction == "CPX") {
                opcode = CPX_ABSOLUTE;
            } else if(instruction == "CPY") {
                opcode = CPY_ABSOLUTE;
            } else if(instruction == "DEC") {
                opcode = DEC_ABSOLUTE;
            } else if(instruction == "EOR") {
                opcode = EOR_ABSOLUTE;
            } else if(instruction == "INC") {
                opcode = INC_ABSOLUTE;
            } else if(instruction == "JMP") {
                opcode = JMP_ABSOLUTE;
            } else if(instruction == "JSR") {
                opcode = JSR_ABSOLUTE;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTE;
            } else if(instruction == "LDX") {
                opcode = LDX_ABSOLUTE;
            } else if(instruction == "LDY") {
                opcode = LDY_ABSOLUTE;
            } else if(instruction == "LSR") {
                opcode = LSR_ABSOLUTE;
            } else if(instruction == "ORA") {
                opcode = ORA_ABSOLUTE;
            } else if(instruction == "ROL") {
                opcode = ROL_ABSOLUTE;
            } else if(instruction == "ROR") {
                opcode = ROR_ABSOLUTE;
            } else if(instruction == "SBC") {
                opcode = SBC_ABSOLUTE;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTE;
            } else if(instruction == "STX") {
                opcode = STX_ABSOLUTE;
            } else if(instruction == "STY") {
                opcode = STY_ABSOLUTE;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(util.getLowByte(arg), util.getHighByte(arg), programLocation);
            programLocation += 2; //because we move two spots in storeWordInMemory

            break;
        case ABSOLUTEX:
            if(instruction == "ADC") {
                opcode = ADC_ABSOLUTEX;
            } else if(instruction == "AND") {
                opcode = AND_ABSOLUTEX;
            } else if(instruction == "ASL") {
                opcode = ASL_ABSOLUTEX;
            } else if(instruction == "CMP") {
                opcode = CMP_ABSOLUTEX;
            } else if(instruction == "DEC") {
                opcode = DEC_ABSOLUTEX;
            } else if(instruction == "EOR") {
                opcode = EOR_ABSOLUTEX;
            } else if(instruction == "INC") {
                opcode = INC_ABSOLUTEX;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTEX;
            } else if(instruction == "LDY") {
                opcode = LDY_ABSOLUTEX;
            } else if(instruction == "LSR") {
                opcode = LSR_ABSOLUTEX;
            } else if(instruction == "ORA") {
                opcode = ORA_ABSOLUTEX;
            } else if(instruction == "ROL") {
                opcode = ROL_ABSOLUTEX;
            } else if(instruction == "ROR") {
                opcode = ROR_ABSOLUTEX;
            } else if(instruction == "SBC") {
                opcode = SBC_ABSOLUTEX;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTEX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(util.getLowByte(arg), util.getHighByte(arg), programLocation);
            programLocation += 2;
            break;
        case ABSOLUTEY:
            if(instruction == "ADC") {
                opcode = ADC_ABSOLUTEY;
            } else if(instruction == "AND") {
                opcode = AND_ABSOLUTEY;
            } else if(instruction == "CMP") {
                opcode = CMP_ABSOLUTEY;
            } else if(instruction == "EOR") {
                opcode = EOR_ABSOLUTEY;
            } else if(instruction == "LDA") {
                opcode = LDA_ABSOLUTEY;
            } else if(instruction == "LDX") {
                opcode = LDX_ABSOLUTEY;
            } else if(instruction == "ORA") {
                opcode = ORA_ABSOLUTEY;
            } else if(instruction == "SBC") {
                opcode = SBC_ABSOLUTEY;
            } else if(instruction == "STA") {
                opcode = STA_ABSOLUTEY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(util.getLowByte(arg), util.getHighByte(arg), programLocation);
            programLocation += 2;
            break;
        case INDEXED_INDIRECTX:
            if(instruction == "ADC") {
                opcode = ADC_INDEXED_INDIRECTX;
            } else if(instruction == "AND") {
                opcode = AND_INDEXED_INDIRECTX;
            } else if(instruction == "CMP") {
                opcode = CMP_INDEXED_INDIRECTX;
            } else if(instruction == "EOR") {
                opcode = EOR_INDEXED_INDIRECTX;
            } else if(instruction == "LDA") {
                opcode = LDA_INDEXED_INDIRECTX;
            } else if(instruction == "ORA") {
                opcode = ORA_INDEXED_INDIRECTX;
            } else if(instruction == "SBC") {
                opcode = SBC_INDEXED_INDIRECTX;
            } else if(instruction == "STA") {
                opcode = STA_INDEXED_INDIRECTX;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);
            break;
        case INDIRECT_INDEXEDY:
            if(instruction == "ADC") {
                opcode = ADC_INDIRECT_INDEXEDY;
            } else if(instruction == "AND") {
                opcode = AND_INDIRECT_INDEXEDY;
            } else if(instruction == "CMP") {
                opcode = CMP_INDIRECT_INDEXEDY;
            } else if(instruction == "EOR") {
                opcode = EOR_INDIRECT_INDEXEDY;
            } else if(instruction == "LDA") {
                opcode = LDA_INDIRECT_INDEXEDY;
            } else if(instruction == "ORA") {
                opcode = ORA_INDIRECT_INDEXEDY;
            } else if(instruction == "SBC") {
                opcode = SBC_INDIRECT_INDEXEDY;
            } else if(instruction == "STA") {
                opcode = STA_INDIRECT_INDEXEDY;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeByteInMemory(util.getLowByte(arg), programLocation++);
            break;
        case INDIRECT:
            if(instruction == "JMP") {
                opcode = JMP_INDIRECT;
            }

            cpu->storeByteInMemory(opcode, programLocation++);
            cpu->storeWordInMemory(util.getLowByte(arg), util.getHighByte(arg), programLocation);
            programLocation += 2;
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

bool Assembler::isInstruction(string word) {
    if(word != "") {
        for(int i = 0; i < invalidLabels.size(); i++) {
            string instruction = invalidLabels[i];

            if(word == instruction) {
                return true;
            }
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




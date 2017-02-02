//
// Created by Christian Curry on 3/5/16.
//

#include <sstream>
#include "Util.h"

using namespace std;

void Util::printByte(uint8_t byte) {
    cout << "$" << setfill('0') << setw(2) << hex << static_cast<int>(byte);
}

void Util::printWord(uint16_t word) {
    cout << "$" << setfill('0') << setw(4) << hex << static_cast<int>(word);
}

bool Util::isNegativeByte(uint8_t c) {
    bool isNegative = (c >> 0) & 128;
    return isNegative;
}

bool Util::isNegativeWord(uint16_t c) {
    bool isNegative = (c >> 0) & 65536;
    return isNegative;
}

uint8_t Util::convertTwosComplement(uint8_t argument) {
    argument = 0xFF - argument;
    argument += 1;

    return argument;
}

bool Util::checkLeastSignificantBit(uint8_t c) {
    bool isSet = (c >> 0) & 1;
    return isSet;
}

//0 checks the least significant bit
bool Util::checkBit(uint8_t argument, int bit) {
    bool isSet = false;

    switch(bit) {
        case 0:
            isSet = argument & 1;
            break;
        case 1:
            isSet = argument & 2;
            break;
        case 2:
            isSet = argument & 4;
            break;
        case 3:
            isSet = argument & 8;
            break;
        case 4:
            isSet = argument & 16;
            break;
        case 5:
            isSet = argument & 32;
            break;
        case 6:
            isSet = argument & 64;
            break;
        case 7:
            isSet = argument & 128;
            break;
    }

    return isSet;
}

uint16_t Util::convertStringToWord(string argument) {
    uint16_t x;

    stringstream ss;
    ss << setfill('0') << setw(4) << hex << argument;
    ss >> x;

    return x;
}

string Util::convertWordToString(uint16_t word) {
    string s;

    stringstream ss;
    ss << hex << word;
    s = ss.str();

    return s;
}

uint16_t Util::getWordFromBytes(uint8_t byteLow, uint8_t byteHigh) {

    //This works because:
    //(0x0002 << 8) | 0x01 = 0x0200 | 0x0001 = 0x0201
    uint16_t word = ((uint16_t)byteHigh << 8) | byteLow;

    return word;
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
uint8_t Util::getLowByte(uint16_t word) {
    uint8_t byte;

    byte = (uint8_t)word;

    return byte;
}

uint8_t Util::getHighByte(uint16_t word) {
    uint8_t byte;

    byte = (uint8_t)(word >> 8);

    return byte;
}

void printAsBinary(uint8_t a)
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1)
        printf("%c",(a&i)?'1':'0');
}

//implied instructions
void Util::printStatus_Implied(uint16_t programCounter, uint8_t opcode, std::string instruction,
                                   uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles) {
    printWord(programCounter);
    cout << "   ";

    printByte(opcode);
    cout << "          ";

    cout << instruction;
    cout << "                                         ";

    cout << "A:";
    printByte(accumulator);
    cout << " ";

    cout << "X:";
    printByte(xIndex);
    cout << " ";

    cout << "Y:";
    printByte(yIndex);
    cout << " ";

    cout << "P:";
    printAsBinary(flags);
    cout << " ";

    cout << "SP:";
    printByte(stackPointer);
    cout << " ";

    cout << "Cycles:" << dec << cycles << " " << endl;
    //switch back to hex so i don't screw anything else up
    cout << hex;
}

//accumulator instructions
void Util::printStatus_Accumulator(uint16_t programCounter, uint8_t opcode, std::string instruction,
                       uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles) {
    printWord(programCounter);
    cout << "   ";

    printByte(opcode);
    cout << " ";

    printExecutedAccumulatorInstruction(instruction);
    cout << "     ";

    cout << "A:";
    printByte(accumulator);
    cout << " ";

    cout << "X:";
    printByte(xIndex);
    cout << " ";

    cout << "Y:";
    printByte(yIndex);
    cout << " ";

    cout << "P:";
    printAsBinary(flags);
    cout << " ";

    cout << "SP:";
    printByte(stackPointer);
    cout << " ";

    cout << "Cycles:" << dec << cycles << " " << endl;
    //switch back to hex so i don't screw anything else up
    cout << hex;
}

//instructions with one argument
void Util::printStatus(uint16_t programCounter, uint8_t opcode, uint8_t argument, std::string instruction,
                       uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles) {
    printWord(programCounter);
    cout << "   ";

    printByte(opcode);
    cout << " ";

    printByte(argument);
    cout << "      ";

    printExecutedByteInstruction(instruction, argument);
    cout << "                                     ";

    cout << "A:";
    printByte(accumulator);
    cout << " ";

    cout << "X:";
    printByte(xIndex);
    cout << " ";

    cout << "Y:";
    printByte(yIndex);
    cout << " ";

    cout << "P:";
    printAsBinary(flags);
    cout << " ";

    cout << "SP:";
    printByte(stackPointer);
    cout << " ";

    cout << "Cycles:" << dec << cycles << endl;
    //switch back to hex so i don't screw anything else up
    cout << hex;
}

//instructions with two arguments
void Util::printStatus(uint16_t programCounter, uint8_t opcode, uint16_t argument, std::string instruction,
                       uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles) {
    uint8_t arg1 = getHighByte(argument);
    uint8_t arg2 = getLowByte(argument);

    printWord(programCounter);
    cout << "   ";

    printByte(opcode);
    cout << " ";

    printByte(arg1);
    cout << " ";

    printByte(arg2);
    cout << "  ";

    printExecutedWordInstruction(instruction, argument);
    cout << "                                   ";

    cout << "A:";
    printByte(accumulator);
    cout << " ";

    cout << "X:";
    printByte(xIndex);
    cout << " ";

    cout << "Y:";
    printByte(yIndex);
    cout << " ";

    cout << "P:";
    printAsBinary(flags);
    cout << " ";

    cout << "SP:";
    printByte(stackPointer);
    cout << " ";

    cout << "Cycles:" << dec << cycles << " " << endl;
    //switch back to hex so i don't screw anything else up
    cout << hex;
}

void Util::printExecutedByteInstruction(string instruction, uint8_t argument) {
    //NOTE: REMOVING THE ADDRESSING MODE FROM THE INSTRUCTION TO MAKE PRINTING EASIER
    //CAN EASILY RESTORE LATER
    instruction = instruction.substr(0, 3);
    cout << instruction << " ";
    printByte(argument);
}

void Util::printExecutedWordInstruction(string instruction, uint16_t argument) {
    //NOTE: REMOVING THE ADDRESSING MODE FROM THE INSTRUCTION TO MAKE PRINTING EASIER
    //CAN EASILY RESTORE LATER
    instruction = instruction.substr(0, 3);
    cout << instruction << " ";
    printWord(argument);
}

void Util::printExecutedAccumulatorInstruction(std::string instruction) {
    cout << instruction << " " << "A";
}

void Util::printMemory(uint16_t start, uint16_t end, uint8_t* memory) {
    int x = 0;
    printf("%04x:  ", start);

    //printf("%04x:  ", start);
    for(int i = start; i <= end; i++) {
        printf("%02x  ", memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
            printf("%04x:  ", i+1);
        }

    }

    cout << endl;
    cout << endl;
}

void Util::printStack(uint8_t* memory) {
    int x = 0;

    for(int i = 0x0100; i <= 0x01ff; i++) {
        printf("%02x ", memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
        }
    }
}
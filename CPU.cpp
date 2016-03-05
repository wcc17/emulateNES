//
// Created by Christian Curry on 3/4/16.
//

#include "CPU.h"
#include "opcodes.h"
#include <iostream>
#include "Util.h"

using namespace std;

CPU::CPU() {
    memory = new uint8_t[65535];
    programCounter = 0x0600;
    programStart = 0x0600;

    accumulator = 0x00;
    xIndex = 0x00;
    yIndex = 0x00;
    stackPointer = 0xff;

//    flags.ignored = 1;
//    flags.breakFlag = 1;
//
//    labelIterator = 0;
}

void CPU::storeByteInMemory(uint8_t byte, uint16_t location) {
    memory[location] = byte;
}

uint16_t CPU::getWordFromBytes(uint8_t byteLow, uint8_t byteHigh) {

    //This works because:
    //(0x0002 << 8) | 0x01 = 0x0200 | 0x0001 = 0x0201
    uint16_t word = ((uint16_t)byteHigh << 8) | byteLow;

    return word;
}

//this will take the op codes from program memory and execute them one at a time
void CPU::executeOpCode() {

    Util util;

    //TODO: IS THIS NECESSARY? I'm not sure that this will happen with 6502 programs that other people wrote
    if(programCounter > 0xFF) {
        programCounter = 0x00;
    }


    //if im passing in straight machine code
    uint8_t opcode = memory[programCounter++];

    //the cases have {} symbols to create a local scope within the case to declare local variables
    switch(opcode) {
        case LDA_IMM:{
            uint8_t argument = memory[programCounter++];

            loadAccumulator_Immediate(argument);

            cout << "LDA_IMM "; util.printByte(argument); cout << endl;
            break;
        }
        case LDA_ABS: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            loadAccumulator_Absolute(argument);

            cout << "LDA_ABS"; util.printWord(argument); cout << endl;
            break;
        }
        case STA_ABS: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            //storeAccumulator_Absolute(argument);

            cout << "STA_ABS"; util.printWord(argument); cout << endl;
            break;
        }
        default:
            cout << "Invalid op code encountered" << endl;
            //to force quit
            memory[programCounter] = 0x00;
            break;
    }
}

void CPU::loadAccumulator_Immediate(uint8_t argument) {
    accumulator = argument;
}

void CPU::loadAccumulator_Absolute(uint16_t argument) {
    accumulator = argument;
}
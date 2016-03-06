//
// Created by Christian Curry on 3/4/16.
//

#include "CPU.h"
#include "opcodes.h"

using namespace std;

CPU::CPU() {
    memory = new uint8_t[65535];
    programCounter = 0x0600;
    programStart = 0x0600;

    accumulator = 0x00;
    xIndex = 0x00;
    yIndex = 0x00;
    stackPointer = 0xff;

    flags.carry = 0;
    flags.interrupt = 0;
    flags.breakFlag = 0;
    flags.zero = 0;
    flags.negative = 0;
    flags.ignored = 1;
    flags.breakFlag = 1;
    flags.decimal = 0;
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
    if(programCounter > 0xFFFF) {
        programCounter = 0x0000;
    }


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

            cout << "LDA_ABS "; util.printWord(argument); cout << endl;
            break;
        }
        case STA_ABS: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            storeAccumulator_Absolute(argument);

            cout << "STA_ABS "; util.printWord(argument); cout << endl;
            break;
        }
        default:
            cout << "Invalid op code encountered" << endl;
            //to force quit
            memory[programCounter] = 0x00;
            break;
    }
}

//acumulator affects sign flag and zero flag
void CPU::loadAccumulator_Immediate(uint8_t argument) {
    accumulator = argument;

    if(accumulator == 0x00) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(argument) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

void CPU::loadAccumulator_Absolute(uint16_t argument) {
    accumulator = argument;

    if(accumulator == 0x00) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeWord(argument) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

void CPU::storeAccumulator_Absolute(uint16_t argument) {
    memory[argument] = accumulator;

    //NOTE: STA affects no flags
}
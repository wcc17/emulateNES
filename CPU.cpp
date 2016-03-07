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

    flags.negative = 0;
    flags.overflow = 0;
    flags.ignored = 1;
    flags.breakFlag = 1;
    flags.decimal = 0;
    flags.interrupt = 0;
    flags.zero = 0;
    flags.carry = 0;
}

void CPU::storeByteInMemory(uint8_t byte, uint16_t location) {
    memory[location] = byte;
}

void CPU::storeWordInMemory(uint8_t lowByte, uint8_t highByte, uint16_t location) {
    memory[location++] = lowByte;
    memory[location] = highByte;
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
        case ADC_IMM:{
            uint8_t argument = memory[programCounter++];
            addWithCarry_Immediate(argument);

            cout << "ADC_IMM "; util.printByte(argument); cout << endl;
            break;
        }

        case INX:{
            incrementX();

            cout << "INX " << endl;
            break;
        }

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

        case TAX: {
            transferAccumulatorToX();

            cout << "TAX" << endl;
            break;
        }

        default:
            cout << "Invalid op code encountered" << endl;
            //to force quit
            memory[programCounter] = 0x00;
            break;
    }
}

void CPU::addWithCarry_Immediate(uint8_t argument) {
    uint8_t carry = 0;
    if(flags.carry == 1) {
        carry = 1;
        flags.carry = 0;
    }

    //get result in 16 bit variable
    //if higher than 0xff, figure out carry situation
    //otherwise, put the result in an 8 bit variable and move on
    uint16_t sumWord = accumulator + argument + carry;
    uint8_t sumByte;
    if(sumWord > 0x00FF) {
        sumByte = sumWord - 0x0100;
        flags.carry = 1;
    } else {
        sumByte = sumWord;
    }

    //if the accumulator and the argument are both negative or both positive
    //AND if the sum is the opposite of them, set the overflow flag
    if(util.isNegativeByte(accumulator) == util.isNegativeByte(argument)) {
        if(util.isNegativeByte(accumulator) != util.isNegativeByte(sumByte)) {
            flags.overflow = 1;
        }
    }

    accumulator = sumByte;

//    t = A + M + P.C
//    P.V = (A.7!=t.7) ? 1:0
//    P.N = A.7
//    P.Z = (t==0) ? 1:0
//    IF (P.D)
//    t = bcd(A) + bcd(M) + P.C
//    P.C = (t>99) ? 1:0
//    ELSE
//    P.C = (t>255) ? 1:0
//    A = t & 0xFF
}

void CPU::incrementX() {
    xIndex += 1;

    //NOTE: INX affects negative flag and zero flag
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

//acumulator affects sign flag and zero flag
void CPU::loadAccumulator_Immediate(uint8_t argument) {
    accumulator = argument;

    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

void CPU::loadAccumulator_Absolute(uint16_t argument) {
    accumulator = memory[argument];

    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

void CPU::storeAccumulator_Absolute(uint16_t argument) {
    memory[argument] = accumulator;

    //NOTE: STA affects no flags
}

void CPU::transferAccumulatorToX() {
    xIndex = accumulator;

    //NOTE: TAX affects N and Z flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
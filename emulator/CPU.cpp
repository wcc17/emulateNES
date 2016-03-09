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

//this will take the op codes from program memory and execute them one at a time
void CPU::executeOpCode() {

    Util util;

    //TODO: IS THIS NECESSARY? I'm not sure that this will happen with 6502 programs that other people wrote
    //TODO: i think since programCounter is 16 bit, if it goes above 0xFFFF, it will reset to 0x0000 by itself. needs testing
    if(programCounter > 0xFFFF) {
        programCounter = 0x0000;
    }

    uint8_t opcode = memory[programCounter++];

    //the cases have {} symbols to create a local scope within the case to declare local variables
    switch(opcode) {
        case ADC_IMMEDIATE:{
            uint8_t argument = memory[programCounter++];
            addWithCarry_Immediate(argument);

            printExecutedByteInstruction("ADC_IMM", argument);
            break;
        }
        case ADC_ZEROPAGE: {
            uint8_t argument = memory[programCounter++];
            addWithCarry_ZeroPage(argument);

            printExecutedByteInstruction("ADC_ZEROPAGE", argument);
            break;
        }
        case ADC_ZEROPAGEX: {
            uint8_t argument = memory[programCounter++];
            argument += xIndex;
            addWithCarry_ZeroPageX(argument);

            printExecutedByteInstruction("ADC_ZEROPAGEX", argument);
            break;
        }
        case ADC_ABSOLUTE: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            addWithCarry_Absolute(argument);

            printExecutedWordInstruction("ADC_ABSOLUTE", argument);
            break;
        }
        case ADC_ABSOLUTEX: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += xIndex;
            addWithCarry_AbsoluteX(argument);

            printExecutedWordInstruction("ADC_ABSOLUTEX", argument);
            break;
        }
        case ADC_ABSOLUTEY: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += yIndex;
            addWithCarry_AbsoluteY(argument);

            printExecutedWordInstruction("ADC_ABSOLUTEY", argument);
            break;
        }

        case INX:{
            incrementX();

            cout << "INX" << endl;
            break;
        }

        case LDA_IMMEDIATE:{
            uint8_t argument = memory[programCounter++];

            loadAccumulator_Immediate(argument);

            printExecutedByteInstruction("LDA_IMM", argument);
            break;
        }
        case LDA_ABSOLUTE: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            loadAccumulator_Absolute(argument);

            printExecutedByteInstruction("LDA_ABS", argument);
            break;
        }

        case LDX_IMMEDIATE: {
            uint8_t argument = memory[programCounter++];

            loadXIndex_Immediate(argument);

            printExecutedByteInstruction("LDX_IMM", argument);
            break;
        }
        case LDX_ZEROPAGE: {
            uint8_t argument = memory[programCounter++];

            loadXIndex_ZeroPage(argument);

            printExecutedByteInstruction("LDX_ZEROPAGE", argument);
            break;
        }
        case LDX_ZEROPAGEY: {
            uint8_t argument = memory[programCounter++];

            argument += yIndex;
            loadXIndex_ZeroPageY(argument);

            printExecutedByteInstruction("LDX_ZEROPAGEY", argument);
            break;
        }
        case LDX_ABSOLUTE: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            loadXIndex_Absolute(argument);

            printExecutedWordInstruction("LDX_ABSOLUTE", argument);
            break;
        }
        case LDX_ABSOLUTEY: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += yIndex;
            loadXIndex_AbsoluteY(argument);

            printExecutedWordInstruction("LDX_ABSOLUTELY", argument);
            break;
        }

        case LDY_IMMEDIATE: {
            uint8_t argument = memory[programCounter++];

            loadYIndex_Immediate(argument);

            printExecutedByteInstruction("LDY_IMM", argument);
            break;
        }
        case LDY_ZEROPAGE: {
            uint8_t argument = memory[programCounter++];

            loadYIndex_ZeroPage(argument);

            printExecutedByteInstruction("LDY_ZEROPAGE", argument);
            break;
        }
        case LDY_ZEROPAGEX: {
            uint8_t argument = memory[programCounter++];

            argument += xIndex;
            loadYIndex_ZeroPageX(argument);

            printExecutedByteInstruction("LDY_ZEROPAGEX", argument);
            break;
        }
        case LDY_ABSOLUTE: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            loadYIndex_Absolute(argument);

            printExecutedWordInstruction("LDY_ABSOLUTE", argument);
            break;
        }
        case LDY_ABSOLUTEX: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += xIndex;
            loadYIndex_AbsoluteX(argument);

            printExecutedWordInstruction("LDY_ABSOLUTELX", argument);
            break;
        }

        case STA_ZEROPAGE: {
            uint8_t argument = memory[programCounter++];

            storeAccumulator_ZeroPage(argument);

            printExecutedByteInstruction("STA_ZEROPAGE", argument);
            break;
        }
        case STA_ZEROPAGEX: {
            uint8_t argument = memory[programCounter++];

            argument += xIndex;
            storeAccumulator_ZeroPageX(argument);

            printExecutedByteInstruction("STA_ZEROPAGEX", argument);
            break;
        }
        case STA_ABSOLUTE: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            storeAccumulator_Absolute(argument);

            printExecutedWordInstruction("STA_ABSOLUTE", argument);
            break;
        }
        case STA_ABSOLUTEX: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += xIndex;
            storeAccumulator_Absolute(argument);

            printExecutedWordInstruction("STA_ABSOLUTEX", argument);
            break;
        }
        case STA_ABSOLUTEY: {
            uint8_t byteLow = memory[programCounter++];
            uint8_t byteHigh = memory[programCounter++];
            uint16_t argument;

            argument = getWordFromBytes(byteLow, byteHigh);
            argument += yIndex;
            storeAccumulator_Absolute(argument);

            printExecutedWordInstruction("STA_ABSOLUTELY", argument);
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

void CPU::addWithCarry(uint8_t argument) {
    uint8_t carry = 0;
    if(flags.carry == 1) {
        carry = 1;
        flags.carry = 0;
    }

    //CHECK CARRY FLAG
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

    //CHECK OVERFLOW FLAG
    //if the accumulator and the argument are both negative or both positive
    //AND if the sum is the opposite of them, set the overflow flag
    //the accumulator used here is before the result of this instruction is carried out.
    if(util.isNegativeByte(accumulator) == util.isNegativeByte(argument)) {
        if(util.isNegativeByte(accumulator) != util.isNegativeByte(sumByte)) {
            flags.overflow = 1;
        } else {
            flags.overflow = 0;
        }
    } else {
        flags.overflow = 0;
    }

    //CHECK ZERO FLAG
    if(sumByte == 0x00) {
        flags.zero = 1;
    } else {
        flags.zero = 0;
    }

    //CHECK NEGATIVE FLAG
    if(util.isNegativeByte(sumByte)) {
        flags.negative = 1;
    } else {
        flags.negative = 0;
    }

    accumulator = sumByte;
}
void CPU::addWithCarry_Immediate(uint8_t argument) {
    addWithCarry(argument);
}
void CPU::addWithCarry_ZeroPage(uint8_t argument) {
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_ZeroPageX(uint8_t argument) {
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
//TODO: can probably reuse this absolute method for every absolute, absolutex,y, and zeropage + zeropagex,y
void CPU::addWithCarry_Absolute(uint16_t argument) {
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_AbsoluteX(uint16_t argument) {
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_AbsoluteY(uint16_t argument) {
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}

void CPU::incrementX() {
    xIndex += 1;

    //NOTE: INX affects negative flag and zero flag
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}

void CPU::loadAccumulator(uint8_t argument) {
    accumulator = argument;

    //acumulator affects sign flag and zero flag
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::loadAccumulator_Immediate(uint8_t argument) {
    loadAccumulator(argument);
}
void CPU::loadAccumulator_Absolute(uint16_t argument) {
    loadAccumulator(memory[argument]);
}

void CPU::loadXIndex(uint8_t argument) {
    xIndex = argument;

    //NOTE: loadXIndex affects sign and zero flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::loadXIndex_Immediate(uint8_t argument) {
    loadXIndex(argument);
}
void CPU::loadXIndex_ZeroPage(uint8_t argument) {
    loadXIndex(argument);
}
void CPU::loadXIndex_ZeroPageY(uint8_t argument) {
    loadXIndex(argument);
}
void CPU::loadXIndex_Absolute(uint16_t argument) {
    loadXIndex(memory[argument]);
}
void CPU::loadXIndex_AbsoluteY(uint16_t argument) {
    loadXIndex(memory[argument]);
}

void CPU::loadYIndex(uint8_t argument) {
    yIndex = argument;

    //NOTE: loadYIndex affects sign and zero flags
    if(yIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(yIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::loadYIndex_Immediate(uint8_t argument) {
    loadYIndex(argument);
}
void CPU::loadYIndex_ZeroPage(uint8_t argument) {
    loadYIndex(argument);
}
void CPU::loadYIndex_ZeroPageX(uint8_t argument) {
    loadYIndex(argument);
}
void CPU::loadYIndex_Absolute(uint16_t argument) {
    loadYIndex(memory[argument]);
}
void CPU::loadYIndex_AbsoluteX(uint16_t argument) {
    loadYIndex(memory[argument]);
}


void CPU::storeAccumulator(uint16_t argument) {
    //NOTE: STA affects no flags
    memory[argument] = accumulator;
}
void CPU::storeAccumulator_ZeroPage(uint8_t argument) {
    storeAccumulator(argument);
}
void CPU::storeAccumulator_ZeroPageX(uint8_t argument) {
    storeAccumulator(argument);
}
void CPU::storeAccumulator_Absolute(uint16_t argument) {
    storeAccumulator(argument);
}
void CPU::storeAccumulator_AbsoluteX(uint16_t argument) {
    storeAccumulator(argument);
}
void CPU::storeAccumulator_AbsoluteY(uint16_t argument) {
    storeAccumulator(argument);
}


void CPU::transferAccumulatorToX() {
    xIndex = accumulator;

    //NOTE: TAX affects N and Z flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
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

void CPU::printExecutedByteInstruction(string instruction, uint8_t argument) {
    cout << instruction << " "; util.printByte(argument); cout << endl;
}

void CPU::printExecutedWordInstruction(string instruction, uint16_t argument) {
    cout << instruction << " "; util.printByte(argument); cout << endl;
}
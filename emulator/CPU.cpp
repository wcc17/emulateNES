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

        //ADC
        case ADC_IMMEDIATE:{
            uint8_t argument = retrieveImmediateInstruction("ADC_IMM");
            addWithCarry_Immediate(argument);
            break;
        }
        case ADC_ZEROPAGE: {
            uint8_t argument = retrieveZeroPageInstruction("ADC_ZEROPAGE");
            addWithCarry_ZeroPage(argument);
            break;
        }
        case ADC_ZEROPAGEX: {
            uint8_t argument = retrieveZeroPageXInstruction("ADC_ZEROPAGEX");
            addWithCarry_ZeroPageX(argument);
            break;
        }
        case ADC_ABSOLUTE: {
            uint16_t argument = retrieveAbsoluteInstruction("ADC_ABSOLUTE");
            addWithCarry_Absolute(argument);
            break;
        }
        case ADC_ABSOLUTEX: {
            uint16_t argument = retrieveAbsoluteXInstruction("ADC_ABSOLUTEX");
            addWithCarry_AbsoluteX(argument);
            break;
        }
        case ADC_ABSOLUTEY: {
            uint16_t argument = retrieveAbsoluteYInstruction("ADC_ABSOLUTEY");
            addWithCarry_AbsoluteY(argument);
            break;
        }

        //INX
        case INX:{
            incrementX();
            cout << "INX" << endl;
            break;
        }

        //LDA
        case LDA_IMMEDIATE:{
            uint8_t argument = retrieveImmediateInstruction("LDA_IMM");
            loadAccumulator_Immediate(argument);
            break;
        }
        case LDA_ABSOLUTE: {
            uint16_t argument = retrieveAbsoluteInstruction("LDA_ABSOLUTE");
            loadAccumulator_Absolute(argument);
            break;
        }

        //LDX
        case LDX_IMMEDIATE: {
            uint8_t argument = retrieveImmediateInstruction("LDX_IMM");
            loadXIndex_Immediate(argument);
            break;
        }
        case LDX_ZEROPAGE: {
            uint8_t argument = retrieveZeroPageInstruction("LDX_ZEROPAGE");
            loadXIndex_ZeroPage(argument);
            break;
        }
        case LDX_ZEROPAGEY: {
            uint8_t argument = retrieveZeroPageYInstruction("LDX_ZEROPAGEY");
            loadXIndex_ZeroPageY(argument);
            break;
        }
        case LDX_ABSOLUTE: {
            uint16_t argument = retrieveAbsoluteInstruction("LDX_ABSOLUTE");
            loadXIndex_Absolute(argument);
            break;
        }
        case LDX_ABSOLUTEY: {
            uint16_t argument = retrieveAbsoluteYInstruction("LDX_ABSOLUTEY");
            loadXIndex_AbsoluteY(argument);
            break;
        }

        //LDY
        case LDY_IMMEDIATE: {
            uint8_t argument = retrieveImmediateInstruction("LDY_IMM");
            loadYIndex_Immediate(argument);
            break;
        }
        case LDY_ZEROPAGE: {
            uint8_t argument = retrieveZeroPageInstruction("LDY_ZEROPAGE");
            loadYIndex_ZeroPage(argument);
            break;
        }
        case LDY_ZEROPAGEX: {
            uint8_t argument = retrieveZeroPageXInstruction("LDY_ZEROPAGEX");
            loadYIndex_ZeroPageX(argument);
            break;
        }
        case LDY_ABSOLUTE: {
            uint16_t argument = retrieveAbsoluteInstruction("LDY_ABSOLUTE");
            loadYIndex_Absolute(argument);
            break;
        }
        case LDY_ABSOLUTEX: {
            uint16_t argument = retrieveAbsoluteXInstruction("LDY_ABSOLUTEX");
            loadYIndex_AbsoluteX(argument);
            break;
        }

        //STA
        case STA_ZEROPAGE: {
            uint8_t argument = retrieveZeroPageInstruction("STA_ZEROPAGE");
            storeAccumulator_ZeroPage(argument);
            break;
        }
        case STA_ZEROPAGEX: {
            uint8_t argument = retrieveZeroPageXInstruction("STA_ZEROPAGEX");
            storeAccumulator_ZeroPageX(argument);
            break;
        }
        case STA_ABSOLUTE: {
            uint16_t argument = retrieveAbsoluteInstruction("STA_ABSOLUTE");
            storeAccumulator_Absolute(argument);
            break;
        }
        case STA_ABSOLUTEX: {
            uint16_t argument = retrieveAbsoluteXInstruction("STA_ABSOLUTEX");
            storeAccumulator_Absolute(argument);
            break;
        }
        case STA_ABSOLUTEY: {
            uint16_t argument = retrieveAbsoluteYInstruction("STA_ABSOLUTEY");
            storeAccumulator_Absolute(argument);
            break;
        }

        //TAX
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



uint8_t CPU::retrieveImmediateInstruction(string instructionString) {
    uint8_t argument = memory[programCounter++];
    printExecutedByteInstruction(instructionString, argument);

    return argument;
}

uint8_t CPU::retrieveZeroPageInstruction(string instructionString) {
    uint8_t argument = memory[programCounter++];
    printExecutedByteInstruction(instructionString, argument);

    return argument;
}

uint8_t CPU::retrieveZeroPageXInstruction(string instructionString) {
    uint8_t argument = memory[programCounter++];
    argument += xIndex;
    printExecutedByteInstruction(instructionString, argument);

    return argument;
}

uint8_t CPU::retrieveZeroPageYInstruction(string instructionString) {
    uint8_t argument = memory[programCounter++];
    argument += yIndex;
    printExecutedByteInstruction(instructionString, argument);

    return argument;
}

uint16_t CPU::retrieveAbsoluteInstruction(string instructionString) {
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
    uint16_t argument;

    argument = getWordFromBytes(byteLow, byteHigh);

    printExecutedWordInstruction(instructionString, argument);
    return argument;
}

uint16_t CPU::retrieveAbsoluteXInstruction(string instructionString) {
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
    uint16_t argument;

    argument = getWordFromBytes(byteLow, byteHigh);
    argument += xIndex;

    printExecutedWordInstruction(instructionString, argument);
    return argument;
}

uint16_t CPU::retrieveAbsoluteYInstruction(string instructionString) {
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
    uint16_t argument;

    argument = getWordFromBytes(byteLow, byteHigh);
    argument += yIndex;

    printExecutedWordInstruction(instructionString, argument);
    return argument;
}
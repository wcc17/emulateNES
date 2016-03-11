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
            addWithCarry_Immediate();
            break;
        }
        case ADC_ZEROPAGE: {
            addWithCarry_ZeroPage();
            break;
        }
        case ADC_ZEROPAGEX: {
            addWithCarry_ZeroPageX();
            break;
        }
        case ADC_ABSOLUTE: {
            addWithCarry_Absolute();
            break;
        }
        case ADC_ABSOLUTEX: {
            addWithCarry_AbsoluteX();
            break;
        }
        case ADC_ABSOLUTEY: {
            addWithCarry_AbsoluteY();
            break;
        }

        //AND
        case AND_IMMEDIATE:{
            andWithAccumulator_Immediate();
            break;
        }
        case AND_ZEROPAGE:{
            andWithAccumulator_ZeroPage();
            break;
        }
        case AND_ZEROPAGEX:{
            andWithAccumulator_ZeroPageX();
            break;
        }
        case AND_ABSOLUTE:{
            andWithAccumulator_Absolute();
            break;
        }
        case AND_ABSOLUTEX:{
            andWithAccumulator_AbsoluteX();
            break;
        }
        case AND_ABSOLUTEY:{
            andWithAccumulator_AbsoluteY();
            break;
        }

        //INX
        case INX:{
            incrementX();
            break;
        }

        //LDA
        case LDA_IMMEDIATE:{
            loadAccumulator_Immediate();
            break;
        }
        case LDA_ZEROPAGE: {
            loadAccumulator_ZeroPage();
            break;
        }
        case LDA_ZEROPAGEX: {
            loadAccumulator_ZeroPageX();
            break;
        }
        case LDA_ABSOLUTE: {
            loadAccumulator_Absolute();
            break;
        }
        case LDA_ABSOLUTEX: {
            loadAccumulator_AbsoluteX();
            break;
        }
        case LDA_ABSOLUTEY: {
            loadAccumulator_AbsoluteY();
            break;
        }

        //LDX
        case LDX_IMMEDIATE: {
            loadXIndex_Immediate();
            break;
        }
        case LDX_ZEROPAGE: {
            loadXIndex_ZeroPage();
            break;
        }
        case LDX_ZEROPAGEY: {
            loadXIndex_ZeroPageY();
            break;
        }
        case LDX_ABSOLUTE: {
            loadXIndex_Absolute();
            break;
        }
        case LDX_ABSOLUTEY: {
            loadXIndex_AbsoluteY();
            break;
        }

        //LDY
        case LDY_IMMEDIATE: {
            loadYIndex_Immediate();
            break;
        }
        case LDY_ZEROPAGE: {
            loadYIndex_ZeroPage();
            break;
        }
        case LDY_ZEROPAGEX: {
            loadYIndex_ZeroPageX();
            break;
        }
        case LDY_ABSOLUTE: {
            loadYIndex_Absolute();
            break;
        }
        case LDY_ABSOLUTEX: {
            loadYIndex_AbsoluteX();
            break;
        }

        //STA
        case STA_ZEROPAGE: {
            storeAccumulator_ZeroPage();
            break;
        }
        case STA_ZEROPAGEX: {
            storeAccumulator_ZeroPageX();
            break;
        }
        case STA_ABSOLUTE: {
            storeAccumulator_Absolute();
            break;
        }
        case STA_ABSOLUTEX: {
            storeAccumulator_AbsoluteX();
            break;
        }
        case STA_ABSOLUTEY: {
            storeAccumulator_AbsoluteY();
            break;
        }

        //TAX
        case TAX: {
            transferAccumulatorToX();
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
void CPU::addWithCarry_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("ADC_IMM");
    addWithCarry(argument);
}
void CPU::addWithCarry_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ADC_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ADC_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
//TODO: can probably reuse this absolute method for every absolute, absolutex,y, and zeropage + zeropagex,y
void CPU::addWithCarry_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ADC_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ADC_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}
void CPU::addWithCarry_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("ADC_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);
}

void CPU::andWithAccumulator(uint8_t argument) {
    accumulator = accumulator & argument;

    //AND affects sign flag and zero flag
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::andWithAccumulator_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("AND_IMM");
    andWithAccumulator(argument);
}
void CPU::andWithAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("AND_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);
}
void CPU::andWithAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("AND_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);
}
void CPU::andWithAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);
}
void CPU::andWithAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);
}
void CPU::andWithAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("AND_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);
}

void CPU::incrementX() {
    cout << "INX" << endl;

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
void CPU::loadAccumulator_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("LDA_IMM");
    loadAccumulator(argument);
}
void CPU::loadAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDA_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);
}
void CPU::loadAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDA_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);
}
void CPU::loadAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDA_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);
}
void CPU::loadAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDA_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);
}
void CPU::loadAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDA_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);
}

void CPU::loadXIndex(uint8_t argument) {
    xIndex = argument;

    //NOTE: loadXIndex affects sign and zero flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::loadXIndex_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("LDX_IMM");
    loadXIndex(argument);
}
void CPU::loadXIndex_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDX_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);
}
void CPU::loadXIndex_ZeroPageY() {
    uint8_t argument = retrieveZeroPageYInstruction("LDX_ZEROPAGEY");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);
}
void CPU::loadXIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDX_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);
}
void CPU::loadXIndex_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDX_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);
}

void CPU::loadYIndex(uint8_t argument) {
    yIndex = argument;

    //NOTE: loadYIndex affects sign and zero flags
    if(yIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(yIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
}
void CPU::loadYIndex_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("LDY_IMM");
    loadYIndex(argument);
}
void CPU::loadYIndex_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDY_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);
}
void CPU::loadYIndex_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDY_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);
}
void CPU::loadYIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDY_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);
}
void CPU::loadYIndex_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDY_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);
}

void CPU::storeAccumulator(uint16_t argument) {
    //NOTE: STA affects no flags
    memory[argument] = accumulator;
}
void CPU::storeAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("STA_ZEROPAGE");
    storeAccumulator(argument);
}
void CPU::storeAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("STA_ZEROPAGEX");
    storeAccumulator(argument);
}
void CPU::storeAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("STA_ABSOLUTE");
    storeAccumulator(argument);
}
void CPU::storeAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("STA_ABSOLUTEX");
    storeAccumulator(argument);
}
void CPU::storeAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("STA_ABSOLUTEY");
    storeAccumulator(argument);
}

void CPU::transferAccumulatorToX() {
    cout << "TAX" << endl;

    xIndex = accumulator;

    //NOTE: TAX affects N and Z flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }
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

uint16_t CPU::retrieveIndexedIndirectXInstruction(string instructionString) {
    uint8_t zeroPageLocation = memory[programCounter++];

    zeroPageLocation += xIndex;

    uint8_t lowByte = memory[zeroPageLocation++];
    uint8_t highByte = memory[zeroPageLocation];

    //get the 16 bit value at zeroPageLocation in memory
    uint16_t argument = getWordFromBytes(lowByte, highByte);

    printExecutedWordInstruction(instructionString, argument);
    return argument;
}

uint16_t CPU::retrieveIndirectIndexedYInstruction(string instructionString) {
    uint8_t zeroPageLocation = memory[programCounter++];

    uint8_t lowByte = memory[zeroPageLocation++];
    uint8_t highByte = memory[zeroPageLocation];

    uint16_t argument = getWordFromBytes(lowByte, highByte);
    argument += yIndex;

    printExecutedWordInstruction(instructionString, argument);
    return argument;
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
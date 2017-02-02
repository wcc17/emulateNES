//
// Created by Christian Curry on 2/2/17.
//

#ifndef EMULATENES_CPUUTIL_H
#define EMULATENES_CPUUTIL_H

#include "CPU.h"

using namespace std;

void CPU::retrieveImpliedInstruction(string instructionString) {
    //DEBUG INFO
    this->instructionString = instructionString;
    this->impliedAddressingMode = true;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = false;
}
void CPU::retrieveAccumulatorInstruction(string instructionString) {
    //DEBUG INFO
    this->instructionString = instructionString;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = true;
    this->addressingMode_8 = false;
    this->addressingMode_16 = false;
}
uint8_t CPU::retrieveRelativeInstruction(string instructionString) {
    uint8_t argument = readMemoryLocation(programCounter++);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_8 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = true;
    this->addressingMode_16 = false;

    return argument;
}
uint8_t CPU::retrieveImmediateInstruction(string instructionString) {
    uint8_t argument = readMemoryLocation(programCounter++);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_8 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = true;
    this->addressingMode_16 = false;

    return argument;
}
uint8_t CPU::retrieveZeroPageInstruction(string instructionString) {
    uint8_t argument = readMemoryLocation(programCounter++);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_8 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = true;
    this->addressingMode_16 = false;

    return argument;
}
uint8_t CPU::retrieveZeroPageXInstruction(string instructionString) {
    uint8_t argument = readMemoryLocation(programCounter++);
    argument += xIndex;

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_8 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = true;
    this->addressingMode_16 = false;

    return argument;
}
uint8_t CPU::retrieveZeroPageYInstruction(string instructionString) {
    uint8_t argument = readMemoryLocation(programCounter++);
    argument += yIndex;

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_8 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = true;
    this->addressingMode_16 = false;

    return argument;
}
uint16_t CPU::retrieveAbsoluteInstruction(string instructionString) {
    uint8_t byteLow = readMemoryLocation(programCounter++);
    uint8_t byteHigh = readMemoryLocation(programCounter++);
    uint16_t argument;

    argument = util.getWordFromBytes(byteLow, byteHigh);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}
uint16_t CPU::retrieveAbsoluteXInstruction(string instructionString) {
    uint16_t startPage;
    uint8_t byteLow = readMemoryLocation(programCounter++);
    uint8_t byteHigh = readMemoryLocation(programCounter++);
    uint16_t argument;

    argument = util.getWordFromBytes(byteLow, byteHigh);
    startPage = argument & 0xFF00;  //get the high byte (the page)
    argument += xIndex;

    //check for page boundary crossing
    if(startPage != (argument & 0xFF00)) {
        pageBoundaryCrossed = true;
    }

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}
uint16_t CPU::retrieveAbsoluteYInstruction(string instructionString) {
    uint16_t startPage;
    uint8_t byteLow = readMemoryLocation(programCounter++);
    uint8_t byteHigh = readMemoryLocation(programCounter++);
    uint16_t argument;

    argument = util.getWordFromBytes(byteLow, byteHigh);
    startPage = argument & 0xFF00;  //get the high byte (the page)
    argument += yIndex;

    //check for page boundary crossing
    if(startPage != (argument & 0xFF00)) {
        pageBoundaryCrossed = true;
    }

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}
uint16_t CPU::retrieveIndexedIndirectXInstruction(string instructionString) {
    uint8_t zeroPageLocation = readMemoryLocation(programCounter++);

    zeroPageLocation += xIndex;

    uint8_t lowByte = readMemoryLocation(zeroPageLocation++);
    uint8_t highByte = readMemoryLocation(zeroPageLocation);

    //get the 16 bit value at zeroPageLocation in memory
    uint16_t argument = util.getWordFromBytes(lowByte, highByte);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}
uint16_t CPU::retrieveIndirectIndexedYInstruction(string instructionString) {
    uint16_t startPage;
    uint8_t zeroPageLocation = readMemoryLocation(programCounter++);

    uint8_t lowByte = readMemoryLocation(zeroPageLocation++);
    uint8_t highByte = readMemoryLocation(zeroPageLocation);

    uint16_t argument = util.getWordFromBytes(lowByte, highByte);
    startPage = argument & 0xFF00; //get the high byte (the page)
    argument += yIndex;

    //check for page boundary crossing
    if(startPage != (argument & 0xF00)) {
        pageBoundaryCrossed = true;
    }

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}
uint16_t CPU::retrieveIndirectInstruction(string instructionString) {

    uint8_t indirectByteLow = readMemoryLocation(programCounter++);
    uint8_t indirectByteHigh = readMemoryLocation(programCounter++);

    //replicating "famous 6502 indirect jump bug"
    //http://everything2.com/title/6502+indirect+JMP+bug
    //if we were to do JMP($C0FF), the byteHigh variable will not be fetched from $C100 as expected, but instead from $C000
    //so the solution here is to build "indirectAddress twice", incrementing the indirectByteLow variable seperately
    //this will let the indirectByteLow wrap around to 00 if its at ff
    uint16_t indirectAddress;
    uint8_t byteLow;
    uint8_t byteHigh;

    indirectAddress = util.getWordFromBytes(indirectByteLow, indirectByteHigh);
    byteLow = readMemoryLocation(indirectAddress);

    indirectByteLow++;
    indirectAddress = util.getWordFromBytes(indirectByteLow, indirectByteHigh);
    byteHigh = readMemoryLocation(indirectAddress);

    uint16_t argument;
    argument = util.getWordFromBytes(byteLow, byteHigh);

    //DEBUG INFO
    this->instructionString = instructionString;
    this->arg_16 = argument;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = true;

    return argument;
}

uint8_t CPU::getProcessorFlagsAsByte() {
    uint8_t processorFlags = ZERO;

    if(flags.negative == 1) { processorFlags = processorFlags | 128; }
    if(flags.overflow == 1) { processorFlags = processorFlags | 64; }
    if(flags.ignored == 1) { processorFlags = processorFlags | 32; }
    if(flags.breakFlag == 1) { processorFlags = processorFlags | 16; }
    if(flags.decimal == 1) { processorFlags = processorFlags | 8; }
    if(flags.interrupt == 1) { processorFlags = processorFlags | 4; }
    if(flags.zero == 1) { processorFlags = processorFlags | 2; }
    if(flags.carry == 1) { processorFlags = processorFlags | 1; }


    return processorFlags;
}
void CPU::setProcessorFlagsFromByte(uint8_t processorStatus) {

    if((processorStatus & 128) != 0) { flags.negative = 1; } else { flags.negative = 0; }
    if((processorStatus & 64) != 0) { flags.overflow = 1; } else { flags.overflow = 0; }
    //break and ignored are not affected. skip 32
    if((processorStatus & 16) != 0) { flags.breakFlag = 1; } else { flags.breakFlag = 0; }
    if((processorStatus & 8) != 0) { flags.decimal = 1; } else { flags.decimal = 0; }
    if((processorStatus & 4) != 0) { flags.interrupt = 1; } else { flags.interrupt = 0; }
    if((processorStatus & 2) != 0) { flags.zero = 1; } else { flags.zero = 0; }
    if((processorStatus & 1) != 0) { flags.carry = 1; } else { flags.carry = 0; }

    //these needs to always be set
    flags.ignored = 1;
}

//stack utility functions
void CPU::pushByte(uint8_t byteToPush) {
    writeMemoryLocation(BASE_STACK_LOCATION + stackPointer--, byteToPush);
}
uint8_t CPU::pullByte() {
    return (readMemoryLocation(BASE_STACK_LOCATION + (++stackPointer)));
}
void CPU::pushWord(uint16_t wordToPush) {
    writeMemoryLocation(BASE_STACK_LOCATION + stackPointer, (wordToPush >> 8) & 0xFF);
    writeMemoryLocation(BASE_STACK_LOCATION + ((stackPointer - 1) & 0xFF), wordToPush & 0xFF);
    stackPointer -= 2;
}
uint16_t CPU::pullWord() {
    uint16_t temp16;
    temp16 = readMemoryLocation(BASE_STACK_LOCATION + ((stackPointer + 1) & 0xFF))
             | ((uint16_t)readMemoryLocation(BASE_STACK_LOCATION + ((stackPointer + 2) & 0xFF)) << 8);
    stackPointer += 2;
    return(temp16);
}

//ONLY used by the assembler, won't use for instructions
void CPU::storeByteInMemory(uint8_t byte, uint16_t location) {
    writeMemoryLocation(location, byte);
}
void CPU::storeWordInMemory(uint8_t lowByte, uint8_t highByte, uint16_t location) {
    writeMemoryLocation(location++, lowByte);
    writeMemoryLocation(location, highByte);
}

//uncomment writeMemoryLocationDefault and comment everything else out to get default CPU behavior. will need a better solution later
void CPU::writeMemoryLocation(uint16_t address, uint8_t value) {
    ram->writeMemoryLocation(address, value);
}
uint8_t CPU::readMemoryLocation(uint16_t address) {
    return ram->readMemoryLocation(address);
}

//simply prints the CPU status after an instruction has been executed
//the programCounter that is shown is the value of the program counter when the previous instruction is retrieved
void CPU::printCPUStatus() {
    if(this->impliedAddressingMode) {
        util.printStatus_Implied(oldPC, opcode, instructionString, accumulator, xIndex, yIndex,
                                 getProcessorFlagsAsByte(), stackPointer, cycleGoal);
    }
    else if(this->accumulatorAddressingMode) {
        util.printStatus_Accumulator(oldPC, opcode, instructionString, accumulator, xIndex, yIndex,
                                     getProcessorFlagsAsByte(), stackPointer, cycleGoal);
    } else if(this->addressingMode_8) {
        util.printStatus(oldPC, opcode, arg_8, instructionString, accumulator, xIndex, yIndex,
                         getProcessorFlagsAsByte(), stackPointer, cycleGoal);
    } else if(this->addressingMode_16) {
        util.printStatus(oldPC, opcode, arg_16, instructionString, accumulator, xIndex, yIndex,
                         getProcessorFlagsAsByte(), stackPointer, cycleGoal);
    }
}

void CPU::printDebugInformation() {
    cout << "Results of previous instruction: " << endl;
    //display results of previous instruction and argument
    cout << "Program Counter: ";
    util.printWord(this->programCounter);
    cout << endl;

    cout << "Accumulator: ";
    util.printByte(this->accumulator);
    cout << endl;

    cout << "xIndex: ";
    util.printByte(this->xIndex);
    cout << endl;

    cout << "yIndex: ";
    util.printByte(this->yIndex);
    cout << endl;

    cout << "Stack Pointer: ";
    util.printByte(this->stackPointer);
    cout << endl;

    cout << "N V - B D I Z C" << endl;
    printf("%d %d %d %d %d %d %d %d\n", this->flags.negative, this->flags.overflow, this->flags.ignored,
           this->flags.breakFlag, this->flags.decimal, this->flags.interrupt, this->flags.zero, this->flags.carry);

    cout << "Stack: " << endl;
    util.printStack(this->ram->memory);
    cout << endl;

//    cout << "Next instruction results to be printed: " << endl << endl;
}

//DEFAULT 6502 BEHAVIOR
//void CPU::writeMemoryLocationDefault(uint16_t address, uint8_t value) {
//    memory[address] = value;
//}
//uint8_t CPU::readMemoryLocationDefault(uint16_t address) {
//    return memory[address];
//}

#endif //EMULATENES_CPUUTIL_H

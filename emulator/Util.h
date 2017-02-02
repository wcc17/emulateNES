//
// Created by Christian Curry on 3/5/16.
//

#ifndef EMULATE6502_UTIL_H
#define EMULATE6502_UTIL_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <iomanip>
#include <iostream>

class Util {
public:
    void printByte(uint8_t byte);
    void printWord(uint16_t word);
    bool isNegativeByte(uint8_t c);
    bool isNegativeWord(uint16_t c);
    uint8_t convertTwosComplement(uint8_t argument);
    bool checkLeastSignificantBit(uint8_t c);
    uint16_t convertStringToWord(std::string argument);
    uint16_t getWordFromBytes(uint8_t byteLow, uint8_t byteHigh);
    std::string convertWordToString(uint16_t word);
    uint8_t getLowByte(uint16_t word);
    uint8_t getHighByte(uint16_t word);

    //0 is the least significant bit
    bool checkBit(uint8_t argument, int bit);

    void printExecutedByteInstruction(std::string instruction, uint8_t argument);
    void printExecutedWordInstruction(std::string instruction, uint16_t argument);
    void printExecutedAccumulatorInstruction(std::string instruction);

    void printStatus_Implied(uint16_t programCounter, uint8_t opcode, std::string instruction,
                                 uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles);
    void printStatus_Accumulator(uint16_t programCounter, uint8_t opcode, std::string instruction,
                     uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles);
    void printStatus(uint16_t programCounter, uint8_t opcode, uint8_t argument, std::string instruction,
                        uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles);
    void printStatus(uint16_t programCounter, uint8_t opcode, uint16_t argument, std::string instruction,
                        uint8_t accumulator, uint8_t xIndex, uint8_t yIndex, uint8_t flags, uint8_t stackPointer, int cycles);

    void printStack(uint8_t* memory);
    void printMemory(uint16_t start, uint16_t end, uint8_t* memory);
};


#endif //EMULATE6502_UTIL_H

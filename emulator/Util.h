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
    void printExecutedByteInstruction(std::string instruction, uint8_t argument);
    void printExecutedWordInstruction(std::string instruction, uint16_t argument);
    void printExecutedAccumulatorInstruction(std::string instruction);
};


#endif //EMULATE6502_UTIL_H

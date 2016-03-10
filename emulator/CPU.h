//
// Created by Christian Curry on 3/4/16.
//

#ifndef EMULATE6502_CPU_H
#define EMULATE6502_CPU_H


#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include "Util.h"

class CPU {

public:
    //TODO: maybe move this to Util?
    static const uint8_t ZERO = 0x00;

    uint8_t* memory;
    uint16_t programCounter;
    uint8_t accumulator;
    uint8_t xIndex;
    uint8_t yIndex;
    uint8_t stackPointer;
    uint16_t programStart;

    struct {
        uint8_t negative : 1;   //S
        uint8_t overflow : 1;   //V
        uint8_t ignored : 1;    //
        uint8_t breakFlag : 1;  //B
        uint8_t decimal : 1;    //D
        uint8_t interrupt : 1;  //I
        uint8_t zero : 1;       //Z
        uint8_t carry : 1;      //C
    } flags;

    CPU();
    void executeOpCode();
    void storeByteInMemory(uint8_t byte, uint16_t location);
    void storeWordInMemory(uint8_t lowByte, uint8_t highByte, uint16_t location);

//private: i would like for these to be private but im not sure of a way around it with testing
    Util util;

    void addWithCarry(uint8_t argument);
    void addWithCarry_Immediate(uint8_t argument);
    void addWithCarry_ZeroPage(uint8_t argument);
    void addWithCarry_ZeroPageX(uint8_t argument);
    void addWithCarry_Absolute(uint16_t argument);
    void addWithCarry_AbsoluteX(uint16_t argument);
    void addWithCarry_AbsoluteY(uint16_t argument);

    void incrementX();

    void loadAccumulator(uint8_t argument);
    void loadAccumulator_Immediate(uint8_t argument);
    void loadAccumulator_ZeroPage(uint8_t argument);
    void loadAccumulator_Absolute(uint16_t argument);

    void loadXIndex(uint8_t argument);
    void loadXIndex_Immediate(uint8_t argument);
    void loadXIndex_ZeroPage(uint8_t argument);
    void loadXIndex_ZeroPageY(uint8_t argument);
    void loadXIndex_Absolute(uint16_t argument);
    void loadXIndex_AbsoluteY(uint16_t argument);

    void loadYIndex(uint8_t argument);
    void loadYIndex_Immediate(uint8_t argument);
    void loadYIndex_ZeroPage(uint8_t argument);
    void loadYIndex_ZeroPageX(uint8_t argument);
    void loadYIndex_Absolute(uint16_t argument);
    void loadYIndex_AbsoluteX(uint16_t argument);

    void storeAccumulator(uint16_t argument);
    void storeAccumulator_ZeroPage(uint8_t argument);
    void storeAccumulator_ZeroPageX(uint8_t argument);
    void storeAccumulator_Absolute(uint16_t argument);
    void storeAccumulator_AbsoluteX(uint16_t argument);
    void storeAccumulator_AbsoluteY(uint16_t argument);

    void transferAccumulatorToX();

private:
    void printExecutedByteInstruction(std::string instruction, uint8_t argument);
    void printExecutedWordInstruction(std::string instruction, uint16_t argument);

    uint16_t getWordFromBytes(uint8_t byteLow, uint8_t byteHigh);

    uint8_t retrieveImmediateInstruction(std::string instructionString);
    uint8_t retrieveZeroPageInstruction(std::string instructionString);
    uint8_t retrieveZeroPageXInstruction(std::string instructionString);
    uint8_t retrieveZeroPageYInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteXInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteYInstruction(std::string instructionString);
};


#endif //EMULATE6502_CPU_H

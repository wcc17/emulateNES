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
    void addWithCarry_Immediate();
    void addWithCarry_ZeroPage();
    void addWithCarry_ZeroPageX();
    void addWithCarry_Absolute();
    void addWithCarry_AbsoluteX();
    void addWithCarry_AbsoluteY();
    void addWithCarry_IndexedIndirectX();
    void addWithCarry_IndirectIndexedY();

    void andWithAccumulator(uint8_t argument);
    void andWithAccumulator_Immediate();
    void andWithAccumulator_ZeroPage();
    void andWithAccumulator_ZeroPageX();
    void andWithAccumulator_Absolute();
    void andWithAccumulator_AbsoluteX();
    void andWithAccumulator_AbsoluteY();
    void andWithAccumulator_IndexedIndirectX();
    void andWithAccumulator_IndirectIndexedY();

    void arithmeticShiftLeft(uint16_t argument, bool useAccumulator);
    void arithmeticShiftLeft_Accumulator();
    void arithmeticShiftLeft_ZeroPage();
    void arithmeticShiftLeft_ZeroPageX();
    void arithmeticShiftLeft_Absolute();
    void arithmeticShiftLeft_AbsoluteX();

    void bitTest(uint8_t argument);
    void bitTest_ZeroPage();
    void bitTest_Absolute();

    void breakInstruction();

    void incrementX();

    void loadAccumulator(uint8_t argument);
    void loadAccumulator_Immediate();
    void loadAccumulator_ZeroPage();
    void loadAccumulator_ZeroPageX();
    void loadAccumulator_Absolute();
    void loadAccumulator_AbsoluteX();
    void loadAccumulator_AbsoluteY();
    void loadAccumulator_IndexedIndirectX();
    void loadAccumulator_IndirectIndexedY();

    void loadXIndex(uint8_t argument);
    void loadXIndex_Immediate();
    void loadXIndex_ZeroPage();
    void loadXIndex_ZeroPageY();
    void loadXIndex_Absolute();
    void loadXIndex_AbsoluteY();

    void loadYIndex(uint8_t argument);
    void loadYIndex_Immediate();
    void loadYIndex_ZeroPage();
    void loadYIndex_ZeroPageX();
    void loadYIndex_Absolute();
    void loadYIndex_AbsoluteX();

    void storeAccumulator(uint16_t argument);
    void storeAccumulator_ZeroPage();
    void storeAccumulator_ZeroPageX();
    void storeAccumulator_Absolute();
    void storeAccumulator_AbsoluteX();
    void storeAccumulator_AbsoluteY();
    void storeAccumulator_IndexedIndirectX();
    void storeAccumulator_IndirectIndexedY();

    void transferAccumulatorToX();

private:
    void printExecutedByteInstruction(std::string instruction, uint8_t argument);
    void printExecutedWordInstruction(std::string instruction, uint16_t argument);
    void printExecutedAccumulatorInstruction(std::string instruction);

    uint16_t getWordFromBytes(uint8_t byteLow, uint8_t byteHigh);

    void retrieveAccumulatorInstruction(std::string instructionString);
    uint8_t retrieveImmediateInstruction(std::string instructionString);
    uint8_t retrieveZeroPageInstruction(std::string instructionString);
    uint8_t retrieveZeroPageXInstruction(std::string instructionString);
    uint8_t retrieveZeroPageYInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteXInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteYInstruction(std::string instructionString);
    uint16_t retrieveIndexedIndirectXInstruction(std::string instructionString);
    uint16_t retrieveIndirectIndexedYInstruction(std::string instructionString);
};


#endif //EMULATE6502_CPU_H

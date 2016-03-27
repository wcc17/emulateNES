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

    void branch(uint8_t argument);
    void branchOnPlus();
    void branchOnMinus();
    void branchOnOverflowClear();
    void branchOnOverflowSet();
    void branchOnCarryClear();
    void branchOnCarrySet();
    void branchOnNotEqual();
    void branchOnEqual();

    //TODO: THIS IS NO WHERE NEAR DONE YET
    void breakInstruction();

    void compareAccumulator(uint8_t argument);
    void compareAccumulator_Immediate();
    void compareAccumulator_ZeroPage();
    void compareAccumulator_ZeroPageX();
    void compareAccumulator_Absolute();
    void compareAccumulator_AbsoluteX();
    void compareAccumulator_AbsoluteY();
    void compareAccumulator_IndexedIndirectX();
    void compareAccumulator_IndirectIndexedY();

    void compareX(uint8_t argument);
    void compareX_Immediate();
    void compareX_ZeroPage();
    void compareX_Absolute();

    void compareY(uint8_t argument);
    void compareY_Immediate();
    void compareY_ZeroPage();
    void compareY_Absolute();

    void decrementMemory(uint16_t argument);
    void decrementMemory_ZeroPage();
    void decrementMemory_ZeroPageX();
    void decrementMemory_Absolute();
    void decrementMemory_AbsoluteX();

    void exclusiveOrAccumulator(uint8_t argument);
    void exclusiveOrAccumulator_Immediate();
    void exclusiveOrAccumulator_ZeroPage();
    void exclusiveOrAccumulator_ZeroPageX();
    void exclusiveOrAccumulator_Absolute();
    void exclusiveOrAccumulator_AbsoluteX();
    void exclusiveOrAccumulator_AbsoluteY();
    void exclusiveOrAccumulator_IndexedIndirectX();
    void exclusiveOrAccumulator_IndirectIndexedY();

    void incrementMemory(uint16_t argument);
    void incrementMemory_ZeroPage();
    void incrementMemory_ZeroPageX();
    void incrementMemory_Absolute();
    void incrementMemory_AbsoluteX();

    void clearCarry();
    void setCarry();
    void clearInterrupt();
    void setInterrupt();
    void clearOverflow();
    void clearDecimal();
    void setDecimal();

    void transferAccumulatorToX();
    void transferXToAccumulator();
    void decrementX();
    void incrementX();
    void transferAccumulatorToY();
    void transferYToAccumulator();
    void decrementY();
    void incrementY();

    void jump(uint16_t argument);
    void jump_Absolute();
    void jump_Indirect();

    void jumpToSubroutine_Absolute();

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

    void logicalShiftRight(uint16_t argument, bool useAccumulator);
    void logicalShiftRight_Accumulator();
    void logicalShiftRight_ZeroPage();
    void logicalShiftRight_ZeroPageX();
    void logicalShiftRight_Absolute();
    void logicalShiftRight_AbsoluteX();

    void returnFromSubroutine();

    void storeAccumulator(uint16_t argument);
    void storeAccumulator_ZeroPage();
    void storeAccumulator_ZeroPageX();
    void storeAccumulator_Absolute();
    void storeAccumulator_AbsoluteX();
    void storeAccumulator_AbsoluteY();
    void storeAccumulator_IndexedIndirectX();
    void storeAccumulator_IndirectIndexedY();

    void transferXToStackPointer();
    void transferStackPointerToX();
    void pushAccumulator();
    void pullAccumulator();
    void pushProcessorStatus();
    void pullProcessorStatus();

    void storeXRegister(uint16_t argument);
    void storeXRegister_ZeroPage();
    void storeXRegister_ZeroPageY();
    void storeXRegister_Absolute();

    void storeYRegister(uint16_t argument);
    void storeYRegister_ZeroPage();
    void storeYRegister_ZeroPageX();
    void storeYRegister_Absolute();

private:
    void printExecutedByteInstruction(std::string instruction, uint8_t argument);
    void printExecutedWordInstruction(std::string instruction, uint16_t argument);
    void printExecutedAccumulatorInstruction(std::string instruction);

    uint8_t getProcessorFlagsAsByte();
    void setProcessorFlagsFromByte(uint8_t processorStatus);

    uint16_t getWordFromBytes(uint8_t byteLow, uint8_t byteHigh);

    void retrieveAccumulatorInstruction(std::string instructionString);
    uint8_t retrieveRelativeInstruction(std::string instructionString);
    uint8_t retrieveImmediateInstruction(std::string instructionString);
    uint8_t retrieveZeroPageInstruction(std::string instructionString);
    uint8_t retrieveZeroPageXInstruction(std::string instructionString);
    uint8_t retrieveZeroPageYInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteXInstruction(std::string instructionString);
    uint16_t retrieveAbsoluteYInstruction(std::string instructionString);
    uint16_t retrieveIndexedIndirectXInstruction(std::string instructionString);
    uint16_t retrieveIndirectIndexedYInstruction(std::string instructionString);
    uint16_t retrieveIndirectInstruction(std::string instructionString);
};


#endif //EMULATE6502_CPU_H

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

//I would prefer these to be private but for testing reasons they aren't right now.
//private:
    Util util;

    uint16_t getWordFromBytes(uint8_t byteLow, uint8_t byteHigh);

    void addWithCarry(uint8_t argument);
    void addWithCarry_Immediate(uint8_t argument);
    void addWithCarry_ZeroPage(uint8_t argument);
    void addWithCarry_ZeroPageX(uint8_t argument);
    void addWithCarry_Absolute(uint16_t argument);

    void incrementX();

    void loadAccumulator(uint8_t argument);
    void loadAccumulator_Immediate(uint8_t argument);
    void loadAccumulator_ZeroPage(uint8_t argument);
    void loadAccumulator_Absolute(uint16_t argument);

    void storeAccumulator(uint16_t argument);
    void storeAccumulator_ZeroPage(uint8_t argument);
    void storeAccumulator_Absolute(uint16_t argument);

    void transferAccumulatorToX();
};


#endif //EMULATE6502_CPU_H

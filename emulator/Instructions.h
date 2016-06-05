//
// Created by Christian Curry on 5/29/16.
//

#ifndef EMULATENES_INSTRUCTIONS_H
#define EMULATENES_INSTRUCTIONS_H

#include "CPU.h"
#include <bitset>

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

    cyclesToExecute += 2;
}
void CPU::addWithCarry_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ADC_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    cyclesToExecute += 3;
}
void CPU::addWithCarry_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ADC_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    cyclesToExecute += 4;
}
void CPU::addWithCarry_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ADC_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    cyclesToExecute += 4;
}
void CPU::addWithCarry_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ADC_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::addWithCarry_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("ADC_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::addWithCarry_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("ADC_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    cyclesToExecute += 6;
}
void CPU::addWithCarry_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("ADC_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    addWithCarry(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
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

    cyclesToExecute += 2;
}
void CPU::andWithAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("AND_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    cyclesToExecute += 2;
}
void CPU::andWithAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("AND_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::andWithAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::andWithAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::andWithAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("AND_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::andWithAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("AND_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::andWithAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("AND_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    andWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::arithmeticShiftLeft(uint16_t argument, bool useAccumulator) {
    uint8_t initialValue;
    uint8_t finalValue;

    if(useAccumulator == true) {
        initialValue = accumulator;
        accumulator = accumulator << 1;
        finalValue = accumulator;
    } else {
        uint8_t memoryValue = readMemoryLocation(argument);
        initialValue = memoryValue;

        memoryValue = memoryValue << 1;
        writeMemoryLocation(argument, memoryValue);
        finalValue = readMemoryLocation(argument);
    }

    //NOTE: ASL AFFECTS SIGN, ZERO, AND CARRY FLAGS
    if(finalValue == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(finalValue) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(util.isNegativeByte(initialValue) == false) { flags.carry = 0; } else { flags.carry = 1; }
}
void CPU::arithmeticShiftLeft_Accumulator() {
    retrieveAccumulatorInstruction("ASL_ACCUMULATOR");
    //there are compiler warnings about passing NULL here, but if accumulator is true, it will never be used
    arithmeticShiftLeft(NULL, true);

    cyclesToExecute += 2;
}
void CPU::arithmeticShiftLeft_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ASL_ZEROPAGE");
    arithmeticShiftLeft(argument, false);

    cyclesToExecute += 5;
}
void CPU::arithmeticShiftLeft_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ASL_ZEROPAGEX");
    arithmeticShiftLeft(argument, false);

    cyclesToExecute += 6;
}
void CPU::arithmeticShiftLeft_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ASL_ABSOLUTE");
    arithmeticShiftLeft(argument, false);

    cyclesToExecute += 6;
}
void CPU::arithmeticShiftLeft_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ASL_ABSOLUTEX");
    arithmeticShiftLeft(argument, false);

    cyclesToExecute += 7;
}

void CPU::bitTest(uint8_t argument) {
    std::bitset<8> arg(argument);

    //check 7th bit (negative)
    if(arg.test(0) == 1) { flags.negative = 1; } else { flags.negative = 0; }
    //check 6th bit (overflow)
    if(arg.test(1) == 1) { flags.overflow = 1; } else { flags.overflow = 0; }
    //check if arg && accumulator are 0 (zero)
    if(argument && accumulator == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::bitTest_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("BIT_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    bitTest(memoryValue);

    cyclesToExecute += 3;
}
void CPU::bitTest_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("BIT_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    bitTest(memoryValue);

    cyclesToExecute += 4;
}

void CPU::branch(uint8_t argument) {

    /**
     * In assembly language, you’ll usually use labels with branch instructions.
     * When assembled though, this label is converted to a single-byte relative
     * offset (a number of bytes to go backwards or forwards from the next instruction)
     * so branch instructions can only go forward and back around 256 bytes.
     * This means they can only be used to move around local code.
     * For moving further you’ll need to use the jumping instructions.
     */

    if(util.isNegativeByte(argument)) {
        argument = util.convertTwosComplement(argument);
        programCounter -= argument;
    } else {
        programCounter += argument;
    }
}
void CPU::branchOnPlus() {
    uint8_t argument = retrieveRelativeInstruction("BPL");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    //do branching
    if(flags.negative == 0) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnMinus() {
    uint8_t argument = retrieveRelativeInstruction("BMI");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.negative == 1) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnOverflowClear() {
    uint8_t argument = retrieveRelativeInstruction("BVC");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.overflow == 0) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnOverflowSet() {
    uint8_t argument = retrieveRelativeInstruction("BVS");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.overflow == 1) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnCarryClear() {
    uint8_t argument = retrieveRelativeInstruction("BCC");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.carry == 0) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnCarrySet() {
    uint8_t argument = retrieveRelativeInstruction("BCS");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.carry == 1) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnNotEqual() {
    uint8_t argument = retrieveRelativeInstruction("BNE");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.zero == 0) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}
void CPU::branchOnEqual() {
    uint8_t argument = retrieveRelativeInstruction("BEQ");

    //a branch not taken requires two machine cycles
    cyclesToExecute += 2;

    if(flags.zero == 1) {
        uint16_t oldProgramCounter = programCounter;
        branch(argument);

        //if a page boundary was crossed
        if((oldProgramCounter & 0xFF00) != (programCounter & 0xFF00)) {
            cyclesToExecute += 2; //increase cycle count for branch + penalty for page boundary crossing
        } else {
            cyclesToExecute += 1; //otherwise increase cycle count for the branch happening only
        }
    }
}

//TODO: AM NOT DONE WITH THIS INSTRUCTION YET
void CPU::breakInstruction() {
    retrieveImpliedInstruction("BRK");

    programCounter++;

    //push the incremented programCounter on to the stack
    pushWord(programCounter);

    //push the status flags on to the stack
    uint8_t statusFlags = getProcessorFlagsAsByte();
    pushByte(statusFlags | 0x10);

    programCounter = (uint16_t)readMemoryLocation(0xFFFE) | ((uint16_t)readMemoryLocation(0xFFFF) << 8);

    cyclesToExecute += 7;

}

void CPU::compareAccumulator(uint8_t argument) {
    uint8_t result = accumulator - argument;

    if(util.isNegativeByte(result) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(accumulator >= argument) { flags.carry = 1; } else { flags.carry = 0; }
    if(result == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::compareAccumulator_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("CMP_IMM");
    compareAccumulator(argument);

    cyclesToExecute += 2;
}
void CPU::compareAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("CMP_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("CMP_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::compareAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CMP_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::compareAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("CMP_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::compareAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("CMP_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::compareAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("CMP_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::compareAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("CMP_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::compareX(uint8_t argument) {
    uint8_t result = xIndex - argument;

    if(util.isNegativeByte(result) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(xIndex >= argument) { flags.carry = 1; } else { flags.carry = 0; }
    if(result == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::compareX_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("CPX_IMM");
    compareX(argument);

    cyclesToExecute += 2;
}
void CPU::compareX_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("CPX_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareX(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareX_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CPX_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareX(memoryValue);

    cyclesToExecute += 4;
}

void CPU::compareY(uint8_t argument) {
    uint8_t result = yIndex - argument;

    if(util.isNegativeByte(result) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(yIndex >= argument) { flags.carry = 1; } else { flags.carry = 0; }
    if(result == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::compareY_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("CPY_IMM");
    compareY(argument);

    cyclesToExecute += 2;
}
void CPU::compareY_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("CPY_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareY(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareY_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CPY_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    compareY(memoryValue);

    cyclesToExecute += 4;
}

void CPU::decrementMemory(uint16_t argument) {
    memory[argument]--;

    if(util.isNegativeByte(readMemoryLocation(argument)) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(readMemoryLocation(argument) == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::decrementMemory_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("DEC_ZEROPAGE");
    decrementMemory(argument);

    cyclesToExecute += 5;
}
void CPU::decrementMemory_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("DEC_ZEROPAGEX");
    decrementMemory(argument);

    cyclesToExecute += 6;
}
void CPU::decrementMemory_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("DEC_ABSOLUTE");
    decrementMemory(argument);

    cyclesToExecute += 6;
}
void CPU::decrementMemory_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("DEC_ABSOLUTEX");
    decrementMemory(argument);

    cyclesToExecute += 7;
}

void CPU::exclusiveOrAccumulator(uint8_t argument) {
    accumulator = accumulator ^ argument;

    //NOTE: EOR affects zero and sign flags
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::exclusiveOrAccumulator_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("EOR_IMMEDIATE");
    exclusiveOrAccumulator(argument);

    cyclesToExecute += 2;
}
void CPU::exclusiveOrAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("EOR_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::exclusiveOrAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("EOR_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::exclusiveOrAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("EOR_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::exclusiveOrAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("EOR_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::exclusiveOrAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("EOR_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::exclusiveOrAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("EOR_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::exclusiveOrAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("EOR_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::incrementMemory(uint16_t argument) {
    memory[argument]++;

    if(util.isNegativeByte(readMemoryLocation(argument)) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(readMemoryLocation(argument) == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::incrementMemory_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("INC_ZEROPAGE");
    incrementMemory(argument);

    cyclesToExecute += 5;
}
void CPU::incrementMemory_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("INC_ZEROPAGEX");
    incrementMemory(argument);

    cyclesToExecute += 6;
}
void CPU::incrementMemory_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("INC_ABSOLUTE");
    incrementMemory(argument);

    cyclesToExecute += 6;
}
void CPU::incrementMemory_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("INC_ABSOLUTEX");
    incrementMemory(argument);

    cyclesToExecute += 7;
}

void CPU::clearCarry() {
    retrieveImpliedInstruction("CLC");
    flags.carry = 0;
    cyclesToExecute += 2;
}
void CPU::setCarry() {
    retrieveImpliedInstruction("SEC");
    flags.carry = 1;
    cyclesToExecute += 2;
}

void CPU::clearInterrupt() {
    retrieveImpliedInstruction("CLI");
    flags.interrupt = 0;

    cyclesToExecute += 2;
}
void CPU::setInterrupt() {
    retrieveImpliedInstruction("SEI");
    flags.interrupt = 1;

    cyclesToExecute += 2;
}

void CPU::clearOverflow() {
    retrieveImpliedInstruction("CLV");
    flags.overflow = 0;

    cyclesToExecute += 2;
}

void CPU::clearDecimal() {
    retrieveImpliedInstruction("CLD");
    flags.decimal = 0;

    cyclesToExecute += 2;
}
void CPU::setDecimal() {
    retrieveImpliedInstruction("SED");
    flags.decimal = 1;

    cyclesToExecute += 2;
}

void CPU::transferAccumulatorToX() {
    retrieveImpliedInstruction("TAX");

    xIndex = accumulator;

    //NOTE: TAX affects N and Z flags
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::transferXToAccumulator() {
    retrieveImpliedInstruction("TXA");

    accumulator = xIndex;

    //NOTE: TXA affects N and Z flags
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::decrementX() {
    retrieveImpliedInstruction("DEX");

    xIndex -= 1;

    //NOTE: DEX affects negative flag and zero flag
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::incrementX() {
    retrieveImpliedInstruction("INX");

    xIndex += 1;

    //NOTE: INX affects negative flag and zero flag
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(xIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::transferAccumulatorToY() {
    retrieveImpliedInstruction("TAY");

    yIndex = accumulator;

    //NOTE: TAY affects N and Z flags
    if(yIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(yIndex) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::transferYToAccumulator() {
    retrieveImpliedInstruction("TYA");

    accumulator = yIndex;

    //NOTE: TYA affects N and Z flags
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }

    cyclesToExecute += 2;
}
void CPU::decrementY() {
    retrieveImpliedInstruction("DEY");

    yIndex -= 1;

    //NOTE: DEY affects negative flag and zero flag
    if(yIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(yIndex) == false) { flags.negative = 0; } else { flags.negative = 1;}

    cyclesToExecute += 2;
}
void CPU::incrementY() {
    retrieveImpliedInstruction("INY");

    yIndex += 1;

    //NOTE: INY affects negative flag and zero flag
    if(yIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(yIndex) == false) { flags.negative = 0; } else { flags.negative = 1;}

    cyclesToExecute += 2;
}

void CPU::jump(uint16_t argument) {
    programCounter = argument;
}
void CPU::jump_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("JMP_ABSOLUTE");
    jump(argument);

    cyclesToExecute += 3;
}
void CPU::jump_Indirect() {
    uint16_t argument = retrieveIndirectInstruction("JMP_INDIRECT");
    jump(argument);

    cyclesToExecute += 5;
}

void CPU::jumpToSubroutine_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("JSR_ABSOLUTE");

    uint16_t t = programCounter - 1;
    uint16_t h = t & 0xFF00;    //contains the high byte but in the wrong place to convert to uint8_t

    uint8_t highByte = h >> 8;
    uint8_t lowByte = t & 0xFF;

    pushByte(highByte);
    pushByte(lowByte);

    programCounter = argument;

    cyclesToExecute += 6;
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

    cyclesToExecute += 2;
}
void CPU::loadAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDA_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDA_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDA_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDA_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::loadAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDA_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::loadAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("LDA_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::loadAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("LDA_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
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

    cyclesToExecute += 2;
}
void CPU::loadXIndex_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDX_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadXIndex(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadXIndex_ZeroPageY() {
    uint8_t argument = retrieveZeroPageYInstruction("LDX_ZEROPAGEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadXIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadXIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDX_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadXIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadXIndex_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDX_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadXIndex(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
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

    cyclesToExecute += 2;
}
void CPU::loadYIndex_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LDY_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadYIndex(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadYIndex_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDY_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadYIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadYIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDY_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadYIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadYIndex_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDY_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    loadYIndex(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}

void CPU::logicalShiftRight(uint16_t argument, bool useAccumulator) {
    uint8_t initialValue;
    uint8_t finalValue;

    if(useAccumulator == true) {
        initialValue = accumulator;
        accumulator = accumulator >> 1;
        finalValue = accumulator;
    } else {
        uint8_t memoryValue = readMemoryLocation(argument);
        initialValue = memoryValue;

        memoryValue = memoryValue >> 1;
        writeMemoryLocation(argument, memoryValue);
        finalValue = readMemoryLocation(argument);
    }

    //NOTE: LSR AFFECTS SIGN, ZERO, AND CARRY FLAGS

    /**this is true because the first bit of the value is going to be shifted to the right and a zero
       will be moved in to the most significant bit spot, making negative = 0 always **/
    flags.negative = 0;

    //if the LSB of the inital value is 1, then carry will be 1
    if(util.checkLeastSignificantBit(initialValue) == true) { flags.carry = 1; } else { flags.carry = 0; }

    if(finalValue == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::logicalShiftRight_Accumulator() {
    retrieveAccumulatorInstruction("LSR_ACCUMULATOR");
    //there are compiler warnings about passing NULL here, but if accumulator is true, it will never be used
    logicalShiftRight(NULL, true);

    cyclesToExecute += 2;
}
void CPU::logicalShiftRight_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("LSR_ZEROPAGE");
    logicalShiftRight(argument, false);

    cyclesToExecute += 5;
}
void CPU::logicalShiftRight_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LSR_ZEROPAGEX");
    logicalShiftRight(argument, false);

    cyclesToExecute += 6;
}
void CPU::logicalShiftRight_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LSR_ABSOLUTE");
    logicalShiftRight(argument, false);

    cyclesToExecute += 6;
}
void CPU::logicalShiftRight_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LSR_ABSOLUTEX");
    logicalShiftRight(argument, false);

    cyclesToExecute += 7;
}

void CPU::noOperation() {
    retrieveImpliedInstruction("NOP");

    //affects no flags

    cyclesToExecute += 2;
}

void CPU::returnFromSubroutine() {
    retrieveImpliedInstruction("RTS");

    uint8_t lowByte = pullByte();
    uint8_t highByte = pullByte();

    uint16_t newProgramCounter = util.getWordFromBytes(lowByte, highByte);
    newProgramCounter++;
    programCounter = newProgramCounter;

    cyclesToExecute += 6;
}
void CPU::returnFromInterrupt() {
    retrieveImpliedInstruction("RTI");

    //the documentation was not right about this
    //it was subtracting from the SP when you need to be adding
    setProcessorFlagsFromByte(pullByte());
    programCounter = pullWord();

//    programCounter++;

    cyclesToExecute += 6;
}

void CPU::orWithAccumulator(uint8_t argument) {
    accumulator = accumulator | argument;

    //NOTE: ORA AFFECTS SIGN AND ZERO FLAGS
    if(util.isNegativeByte(accumulator) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::orWithAccumulator_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("ORA_IMMEDIATE");
    orWithAccumulator(argument);

    cyclesToExecute += 2;
}
void CPU::orWithAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ORA_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    cyclesToExecute += 2;
}
void CPU::orWithAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ORA_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::orWithAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ORA_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::orWithAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ORA_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::orWithAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("ORA_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::orWithAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("ORA_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::orWithAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("ORA_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    orWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::rotateLeft(uint16_t argument, bool useAccumulator) {
    uint8_t initialValue;
    uint8_t finalValue;

    /** how this works:
     *  shift all bits one position to the left.
     * the carry is shifted into least significant bit and most significant bit is shifted into carry
     **/

    if(useAccumulator) {
        initialValue = accumulator;
    } else {
        initialValue = readMemoryLocation(argument);
    }

    //if the 7th bit is set (if value is negative) set carry to 1, otherwise set to 0
    bool setCarry = false;
    if(util.isNegativeByte(initialValue)) { setCarry = true; } else { setCarry = false; }

    //get the final value
    finalValue = initialValue << 1;

    //set the LSB based on the carry flag
    if(flags.carry == 1) {
        finalValue = finalValue | 0x01;
    }

    //now change carry based on the original value's MSB
    if(setCarry) { flags.carry = 1; } else { flags.carry = 0; }

    //instruction also affects sign and zero flags
    if(finalValue == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(finalValue)) { flags.negative = 1; } else { flags.negative = 0; }

    //finally, put the value back in memory (or in the accumulator)
    if(useAccumulator) {
        accumulator = finalValue;
    } else {
        writeMemoryLocation(argument, finalValue);
    }

}
void CPU::rotateLeft_Accumulator() {
    retrieveAccumulatorInstruction("ROL_ACCUMULATOR");
    rotateLeft(NULL, true);

    cyclesToExecute += 2;
}
void CPU::rotateLeft_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ROL_ZEROPAGE");
    rotateLeft(argument, false);

    cyclesToExecute += 5;
}
void CPU::rotateLeft_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ROL_ZEROPAGEX");
    rotateLeft(argument, false);

    cyclesToExecute += 6;
}
void CPU::rotateLeft_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ROL_ABSOLUTE");
    rotateLeft(argument, false);

    cyclesToExecute += 6;
}
void CPU::rotateLeft_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ROL_ABSOLUTEX");
    rotateLeft(argument, false);

    cyclesToExecute += 7;
}

void CPU::rotateRight(uint16_t argument, bool useAccumulator) {
    uint8_t initialValue;
    uint8_t finalValue;

    /**
     * Shift all bits one position to the right
     * The carry is shifted into most significant bit
     * least significant bit is shifted into carry
     */

    if(useAccumulator) {
        initialValue = accumulator;
    } else {
        initialValue = readMemoryLocation(argument);
    }

    //if the least significant bit is set, set carry to 1 later, or 0 if otherwise
    bool setCarry = false;
    if(util.checkLeastSignificantBit(initialValue)) {
        setCarry = true;
    } else {
        setCarry = false;
    }

    //get the final value (shift bits to the right 1
    finalValue = initialValue >> 1;

    //set the MSB based on the carry flag
    if(flags.carry == 1) {
        finalValue = finalValue | 0x80; //0x80h == 128 == 10000000b
    }

    //now change carry based on the original values LSB
    if(setCarry) { flags.carry = 1; } else { flags.carry = 0; }

    //instruction also affects sign and zero flags
    if(finalValue == ZERO) { flags.zero = 1; } else { flags.zero = 0; }
    if(util.isNegativeByte(finalValue)) { flags.negative = 1; } else { flags.negative = 0; }

    //put value back in memory or in accumulator
    if(useAccumulator) {
        accumulator = finalValue;
    } else {
        writeMemoryLocation(argument, finalValue);
    }
}
void CPU::rotateRight_Accumulator() {
    retrieveAccumulatorInstruction("ROR_ACCUMULATOR");
    rotateRight(NULL, true);

    cyclesToExecute += 2;
}
void CPU::rotateRight_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("ROR_ZEROPAGE");
    rotateRight(argument, false);

    cyclesToExecute += 5;
}
void CPU::rotateRight_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ROR_ZEROPAGEX");
    rotateRight(argument, false);

    cyclesToExecute += 6;
}
void CPU::rotateRight_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ROR_ABSOLUTE");
    rotateRight(argument, false);

    cyclesToExecute += 6;
}
void CPU::rotateRight_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ROR_ABSOLUTEX");
    rotateRight(argument, false);

    cyclesToExecute += 7;
}

void CPU::storeAccumulator(uint16_t argument) {
    //NOTE: STA affects no flags
    writeMemoryLocation(argument, accumulator);
}
void CPU::storeAccumulator_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("STA_ZEROPAGE");
    storeAccumulator(argument);

    cyclesToExecute += 3;
}
void CPU::storeAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("STA_ZEROPAGEX");
    storeAccumulator(argument);

    cyclesToExecute += 4;
}
void CPU::storeAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("STA_ABSOLUTE");
    storeAccumulator(argument);

    cyclesToExecute += 4;
}
void CPU::storeAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("STA_ABSOLUTEX");
    storeAccumulator(argument);

    cyclesToExecute += 5;
}
void CPU::storeAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("STA_ABSOLUTEY");
    storeAccumulator(argument);

    cyclesToExecute += 5;
}
void CPU::storeAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("STA_INDEXED_INDIRECTX");
    storeAccumulator(argument);

    cyclesToExecute += 6;
}
void CPU::storeAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("STA_INDIRECT_INDEXEDY");
    storeAccumulator(argument);

    cyclesToExecute += 6;
}

void CPU::transferXToStackPointer() {
    retrieveImpliedInstruction("TXS");

    stackPointer = xIndex;

    if(util.isNegativeByte(stackPointer)) { flags.negative = 1; } else { flags.negative = 0; }
    if(stackPointer == ZERO) { flags.zero = 1; } else { flags.zero = 0; }

    cyclesToExecute += 2;
}
void CPU::transferStackPointerToX() {
    retrieveImpliedInstruction("TSX");

    xIndex = stackPointer;

    if(util.isNegativeByte(xIndex)) { flags.negative = 1; } else { flags.negative = 0; }
    if(xIndex == ZERO) { flags.zero = 1; } else { flags.zero = 0; }

    cyclesToExecute += 2;
}
void CPU::pushAccumulator() {
    retrieveImpliedInstruction("PHA");

    //stack pointer = 0xff but actually lives at 0x01ff
    //0xff + 256(0x0100) = 0x1ff
    pushByte(accumulator);

    //NOTE: PHA AFFECTS NO FLAGS

    cyclesToExecute += 3;
}
void CPU::pullAccumulator() {
    retrieveImpliedInstruction("PLA");

    accumulator = pullByte();

    if(util.isNegativeByte(accumulator)) { flags.negative = 1; } else { flags.negative = 0; }
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }

    cyclesToExecute += 4;
}
void CPU::pushProcessorStatus() {
    retrieveImpliedInstruction("PHP");

    //the binary value for the flags need to be pushed onto stack
    uint8_t processorStatus = getProcessorFlagsAsByte();
    pushByte(processorStatus | 0x10); //THE BRK FLAG IS SET BEFORE BEING PUSHED ON TO THE STACK

    //NOTE: PHP AFFECTS NO FLAGS, BUT PLP DOES

    cyclesToExecute += 3;
}
void CPU::pullProcessorStatus() {
    retrieveImpliedInstruction("PLP");

    uint8_t processorStatus = pullByte();
    setProcessorFlagsFromByte(processorStatus);

    //PLP AFFECTS ALL FLAGS, BUT THEY ARE SET IN setProcessorFlagsFromByte(uint8_t arg)

    cyclesToExecute += 4;
}

void CPU::storeXRegister(uint16_t argument) {
    //NOTE: STX affects no flags
    writeMemoryLocation(argument, xIndex);
}
void CPU::storeXRegister_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("STX_ZEROPAGE");
    storeXRegister(argument);

    cyclesToExecute += 3;
}
void CPU::storeXRegister_ZeroPageY() {
    uint8_t argument = retrieveZeroPageYInstruction("STX_ZEROPAGEY");
    storeXRegister(argument);

    cyclesToExecute += 4;
}
void CPU::storeXRegister_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("STX_ABSOLUTE");
    storeXRegister(argument);

    cyclesToExecute += 4;
}

void CPU::storeYRegister(uint16_t argument) {
    //NOTE: STY affects no flags
    writeMemoryLocation(argument, yIndex);
}
void CPU::storeYRegister_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("STY_ZEROPAGE");
    storeYRegister(argument);

    cyclesToExecute += 3;
}
void CPU::storeYRegister_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("STY_ZEROPAGEX");
    storeYRegister(argument);

    cyclesToExecute += 4;
}
void CPU::storeYRegister_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("STY_ABSOLUTE");
    storeYRegister(argument);

    cyclesToExecute += 4;
}

void CPU::subtractWithBorrow(uint8_t argument) {
    /**
     * ELSE
        t = A - M - !P.C
        P.V = (t>127 OR t<-128) ? 1:0
      P.C = (t>=0) ? 1:0
      P.N = t.7
      P.Z = (t==0) ? 1:0
      A = t & 0xFF
     */

    //SBC uses the opposite of the carry
    //~argument inverts all the bits of the argument and adds 1 (twos complement)
    //a subtraction with a twos complement number is just addition
    addWithCarry(~argument);
}
void CPU::subtractWithBorrow_Immediate() {
    uint8_t argument = retrieveImmediateInstruction("SBC_IMM");
    subtractWithBorrow(argument);

    cyclesToExecute += 2;
}
void CPU::subtractWithBorrow_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("SBC_ZEROPAGE");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 3;
}
void CPU::subtractWithBorrow_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("SBC_ZEROPAGEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 4;
}
void CPU::subtractWithBorrow_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("SBC_ABSOLUTE");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 4;
}
void CPU::subtractWithBorrow_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("SBC_ABSOLUTEX");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::subtractWithBorrow_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("SBC_ABSOLUTEY");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::subtractWithBorrow_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("SBC_INDEXED_INDIRECTX");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 6;
}
void CPU::subtractWithBorrow_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("SBC_INDIRECT_INDEXEDY");
    uint8_t memoryValue = readMemoryLocation(argument);
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

#endif //EMULATENES_INSTRUCTIONS_H

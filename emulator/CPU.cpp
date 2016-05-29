//
// Created by Christian Curry on 3/4/16.
//

#include "CPU.h"
#include "opcodes.h"
#include <bitset>

using namespace std;

CPU::CPU() {
    memory = new uint8_t[65535];
    programCounter = 0x0600;
    programStart = 0x0600;

    accumulator = 0x00;
    xIndex = 0x00;
    yIndex = 0x00;
    stackPointer = 0xff;

    //TODO: all flags should be set on startup except for
    flags.negative = 0;
    flags.overflow = 0;
    flags.ignored = 1;
    flags.breakFlag = 1;
    flags.decimal = 0;
    flags.interrupt = 0;
    flags.zero = 0;
    flags.carry = 0;
}

void CPU::execute() {
    //TODO: IS THIS NECESSARY? I'm not sure that this will happen with 6502 programs that other people wrote
    //TODO: i think since programCounter is 16 bit, if it goes above 0xFFFF, it will reset to 0x0000 by itself. needs testing
    if(programCounter > 0xFFFF) {
        programCounter = 0x0000;
    }

    //if an interrupt has been raised, handle it. otherwise, execute next instruction
    //this will have allowed the previous instruction to finish first before doing the interrupt
    if(flags.interrupt == 0 && interruptRaised) {

        //handle interrupt

        interruptRaised = false;
    } else {
        executeOpCode();

        //count cycles
        cycleGoal += cyclesToExecute;
        while(cyclesToExecute > 0) {
            //wait
            cyclesToExecute -= 1;
        }

        //this won't ever be needed after an instruction is executed, but will screw something up if left at true for the next one
        pageBoundaryCrossed = false;
    }
}

//this will take the op codes from program memory and execute them one at a time
void CPU::executeOpCode() {

    oldPC = programCounter;
    opcode = memory[programCounter++];
    //the cases have {} symbols to create a local scope within the case to declare local variables
    switch(opcode) {

        //ADC
        case ADC_IMMEDIATE:
            addWithCarry_Immediate();
            break;
        case ADC_ZEROPAGE:
            addWithCarry_ZeroPage();
            break;
        case ADC_ZEROPAGEX:
            addWithCarry_ZeroPageX();
            break;
        case ADC_ABSOLUTE:
            addWithCarry_Absolute();
            break;
        case ADC_ABSOLUTEX:
            addWithCarry_AbsoluteX();
            break;
        case ADC_ABSOLUTEY:
            addWithCarry_AbsoluteY();
            break;
        case ADC_INDEXED_INDIRECTX:
            addWithCarry_IndexedIndirectX();
            break;
        case ADC_INDIRECT_INDEXEDY:
            addWithCarry_IndirectIndexedY();
            break;

        //AND
        case AND_IMMEDIATE:
            andWithAccumulator_Immediate();
            break;
        case AND_ZEROPAGE:
            andWithAccumulator_ZeroPage();
            break;
        case AND_ZEROPAGEX:
            andWithAccumulator_ZeroPageX();
            break;
        case AND_ABSOLUTE:
            andWithAccumulator_Absolute();
            break;
        case AND_ABSOLUTEX:
            andWithAccumulator_AbsoluteX();
            break;
        case AND_ABSOLUTEY:
            andWithAccumulator_AbsoluteY();
            break;
        case AND_INDEXED_INDIRECTX:
            andWithAccumulator_IndexedIndirectX();
            break;
        case AND_INDIRECT_INDEXEDY:
            andWithAccumulator_IndirectIndexedY();
            break;

        //ASL
        case ASL_ACCUMULATOR:
            arithmeticShiftLeft_Accumulator();
            break;
        case ASL_ZEROPAGE:
            arithmeticShiftLeft_ZeroPage();
            break;
        case ASL_ZEROPAGEX:
            arithmeticShiftLeft_ZeroPageX();
            break;
        case ASL_ABSOLUTE:
            arithmeticShiftLeft_Absolute();
            break;
        case ASL_ABSOLUTEX:
            arithmeticShiftLeft_AbsoluteX();
            break;

        //BIT
        case BIT_ZEROPAGE:
            bitTest_ZeroPage();
            break;
        case BIT_ABSOLUTE:
            bitTest_Absolute();
            break;

        //BRANCH INSTRUCTIONS
        case BPL:
            branchOnPlus();
            break;
        case BMI:
            branchOnMinus();
            break;
        case BVC:
            branchOnOverflowClear();
            break;
        case BVS:
            branchOnOverflowSet();
            break;
        case BCC:
            branchOnCarryClear();
            break;
        case BCS:
            branchOnCarrySet();
            break;
        case BNE:
            branchOnNotEqual();
            break;
        case BEQ:
            branchOnEqual();
            break;

        //BRK
        case BRK:
            breakInstruction();
            break;

        //CMP
        case CMP_IMMEDIATE:
            compareAccumulator_Immediate();
            break;
        case CMP_ZEROPAGE:
            compareAccumulator_ZeroPage();
            break;
        case CMP_ZEROPAGEX:
            compareAccumulator_ZeroPageX();
            break;
        case CMP_ABSOLUTE:
            compareAccumulator_Absolute();
            break;
        case CMP_ABSOLUTEX:
            compareAccumulator_AbsoluteX();
            break;
        case CMP_ABSOLUTEY:
            compareAccumulator_AbsoluteY();
            break;
        case CMP_INDEXED_INDIRECTX:
            compareAccumulator_IndexedIndirectX();
            break;
        case CMP_INDIRECT_INDEXEDY:
            compareAccumulator_IndirectIndexedY();
            break;

        //CPX
        case CPX_IMMEDIATE:
            compareX_Immediate();
            break;
        case CPX_ZEROPAGE:
            compareX_ZeroPage();
            break;
        case CPX_ABSOLUTE:
            compareX_Absolute();
            break;

        //CPY
        case CPY_IMMEDIATE:
            compareY_Immediate();
            break;
        case CPY_ZEROPAGE:
            compareY_ZeroPage();
            break;
        case CPY_ABSOLUTE:
            compareY_Absolute();
            break;

        //DEC
        case DEC_ZEROPAGE:
            decrementMemory_ZeroPage();
            break;
        case DEC_ZEROPAGEX:
            decrementMemory_ZeroPageX();
            break;
        case DEC_ABSOLUTE:
            decrementMemory_Absolute();
            break;
        case DEC_ABSOLUTEX:
            decrementMemory_AbsoluteX();
            break;

        //EOR
        case EOR_IMMEDIATE:
            exclusiveOrAccumulator_Immediate();
            break;
        case EOR_ZEROPAGE:
            exclusiveOrAccumulator_ZeroPage();
            break;
        case EOR_ZEROPAGEX:
            exclusiveOrAccumulator_ZeroPageX();
            break;
        case EOR_ABSOLUTE:
            exclusiveOrAccumulator_Absolute();
            break;
        case EOR_ABSOLUTEX:
            exclusiveOrAccumulator_AbsoluteX();
            break;
        case EOR_ABSOLUTEY:
            exclusiveOrAccumulator_AbsoluteY();
            break;
        case EOR_INDEXED_INDIRECTX:
            exclusiveOrAccumulator_IndexedIndirectX();
            break;
        case EOR_INDIRECT_INDEXEDY:
            exclusiveOrAccumulator_IndirectIndexedY();
            break;

        //INC
        case INC_ZEROPAGE:
            incrementMemory_ZeroPage();
            break;
        case INC_ZEROPAGEX:
            incrementMemory_ZeroPageX();
            break;
        case INC_ABSOLUTE:
            incrementMemory_Absolute();
            break;
        case INC_ABSOLUTEX:
            incrementMemory_AbsoluteX();
            break;

        //Flag Instructions
        case CLC:
            clearCarry();
            break;
        case SEC:
            setCarry();
            break;
        case CLI:
            clearInterrupt();
            break;
        case SEI:
            setInterrupt();
            break;
        case CLV:
            clearOverflow();
            break;
        case CLD:
            clearDecimal();
            break;
        case SED:
            setDecimal();
            break;

        //Register Instructions
        case TAX:
            transferAccumulatorToX();
            break;
        case TXA:
            transferXToAccumulator();
            break;
        case DEX:
            decrementX();
            break;
        case INX:
            incrementX();
            break;
        case TAY:
            transferAccumulatorToY();
            break;
        case TYA:
            transferYToAccumulator();
            break;
        case DEY:
            decrementY();
            break;
        case INY:
            incrementY();
            break;

        //JMP
        case JMP_ABSOLUTE:
            jump_Absolute();
            break;
        case JMP_INDIRECT:
            jump_Indirect();
            break;

        //JSR
        case JSR_ABSOLUTE:
            jumpToSubroutine_Absolute();
            break;

        //LDA
        case LDA_IMMEDIATE:
            loadAccumulator_Immediate();
            break;
        case LDA_ZEROPAGE:
            loadAccumulator_ZeroPage();
            break;
        case LDA_ZEROPAGEX:
            loadAccumulator_ZeroPageX();
            break;
        case LDA_ABSOLUTE:
            loadAccumulator_Absolute();
            break;
        case LDA_ABSOLUTEX:
            loadAccumulator_AbsoluteX();
            break;
        case LDA_ABSOLUTEY:
            loadAccumulator_AbsoluteY();
            break;
        case LDA_INDEXED_INDIRECTX:
            loadAccumulator_IndexedIndirectX();
            break;
        case LDA_INDIRECT_INDEXEDY:
            loadAccumulator_IndirectIndexedY();
            break;

        //LDX
        case LDX_IMMEDIATE:
            loadXIndex_Immediate();
            break;
        case LDX_ZEROPAGE:
            loadXIndex_ZeroPage();
            break;
        case LDX_ZEROPAGEY:
            loadXIndex_ZeroPageY();
            break;
        case LDX_ABSOLUTE:
            loadXIndex_Absolute();
            break;
        case LDX_ABSOLUTEY:
            loadXIndex_AbsoluteY();
            break;

        //LDY
        case LDY_IMMEDIATE:
            loadYIndex_Immediate();
            break;
        case LDY_ZEROPAGE:
            loadYIndex_ZeroPage();
            break;
        case LDY_ZEROPAGEX:
            loadYIndex_ZeroPageX();
            break;
        case LDY_ABSOLUTE:
            loadYIndex_Absolute();
            break;
        case LDY_ABSOLUTEX:
            loadYIndex_AbsoluteX();
            break;

        //LSR
        case LSR_ACCUMULATOR:
            logicalShiftRight_Accumulator();
            break;
        case LSR_ZEROPAGE:
            logicalShiftRight_ZeroPage();
            break;
        case LSR_ZEROPAGEX:
            logicalShiftRight_ZeroPageX();
            break;
        case LSR_ABSOLUTE:
            logicalShiftRight_Absolute();
            break;
        case LSR_ABSOLUTEX:
            logicalShiftRight_AbsoluteX();
            break;

        //NOP
        case NOP:
            noOperation();
            break;

        //RTS
        case RTS:
            returnFromSubroutine();
            break;

        //ORA
        case ORA_IMMEDIATE:
            orWithAccumulator_Immediate();
            break;
        case ORA_ZEROPAGE:
            orWithAccumulator_ZeroPage();
            break;
        case ORA_ZEROPAGEX:
            orWithAccumulator_ZeroPageX();
            break;
        case ORA_ABSOLUTE:
            orWithAccumulator_Absolute();
            break;
        case ORA_ABSOLUTEX:
            orWithAccumulator_AbsoluteX();
            break;
        case ORA_ABSOLUTEY:
            orWithAccumulator_AbsoluteY();
            break;
        case ORA_INDEXED_INDIRECTX:
            orWithAccumulator_IndexedIndirectX();
            break;
        case ORA_INDIRECT_INDEXEDY:
            orWithAccumulator_IndirectIndexedY();
            break;

        //ROL
        case ROL_ACCUMULATOR:
            rotateLeft_Accumulator();
            break;
        case ROL_ZEROPAGE:
            rotateLeft_ZeroPage();
            break;
        case ROL_ZEROPAGEX:
            rotateLeft_ZeroPageX();
            break;
        case ROL_ABSOLUTE:
            rotateLeft_Absolute();
            break;
        case ROL_ABSOLUTEX:
            rotateLeft_AbsoluteX();
            break;

        //ROR
        case ROR_ACCUMULATOR:
            rotateRight_Accumulator();
            break;
        case ROR_ZEROPAGE:
            rotateRight_ZeroPage();
            break;
        case ROR_ZEROPAGEX:
            rotateRight_ZeroPageX();
            break;
        case ROR_ABSOLUTE:
            rotateRight_Absolute();
            break;
        case ROR_ABSOLUTEX:
            rotateRight_AbsoluteX();
            break;

        //SBC
        case SBC_IMMEDIATE:
            subtractWithBorrow_Immediate();
            break;
        case SBC_ZEROPAGE:
            subtractWithBorrow_ZeroPage();
            break;
        case SBC_ZEROPAGEX:
            subtractWithBorrow_ZeroPageX();
            break;
        case SBC_ABSOLUTE:
            subtractWithBorrow_Absolute();
            break;
        case SBC_ABSOLUTEX:
            subtractWithBorrow_AbsoluteX();
            break;
        case SBC_ABSOLUTEY:
            subtractWithBorrow_AbsoluteY();
            break;
        case SBC_INDEXED_INDIRECTX:
            subtractWithBorrow_IndexedIndirectX();
            break;
        case SBC_INDIRECT_INDEXEDY:
            subtractWithBorrow_IndirectIndexedY();
            break;

        //STA
        case STA_ZEROPAGE:
            storeAccumulator_ZeroPage();
            break;
        case STA_ZEROPAGEX:
            storeAccumulator_ZeroPageX();
            break;
        case STA_ABSOLUTE:
            storeAccumulator_Absolute();
            break;
        case STA_ABSOLUTEX:
            storeAccumulator_AbsoluteX();
            break;
        case STA_ABSOLUTEY:
            storeAccumulator_AbsoluteY();
            break;
        case STA_INDEXED_INDIRECTX:
            storeAccumulator_IndexedIndirectX();
            break;
        case STA_INDIRECT_INDEXEDY:
            storeAccumulator_IndirectIndexedY();
            break;

        //Stack Instructions
        case TXS:
            transferXToStackPointer();
            break;
        case TSX:
            transferStackPointerToX();
            break;
        case PHA:
            pushAccumulator();
            break;
        case PLA:
            pullAccumulator();
            break;
        case PHP:
            pushProcessorStatus();
            break;
        case PLP:
            pullProcessorStatus();
            break;

        //STX
        case STX_ZEROPAGE:
            storeXRegister_ZeroPage();
            break;
        case STX_ZEROPAGEY:
            storeXRegister_ZeroPageY();
            break;
        case STX_ABSOLUTE:
            storeXRegister_Absolute();
            break;

        //STY
        case STY_ZEROPAGE:
            storeYRegister_ZeroPage();
            break;
        case STY_ZEROPAGEX:
            storeYRegister_ZeroPageX();
            break;
        case STY_ABSOLUTE:
            storeYRegister_Absolute();
            break;

        default:
            cout << endl;
            cout << "INVALID OP CODE ENCOUNTERED: ";
            util.printByte(opcode);
            cout << endl << endl;
            break;
    }

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
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    cyclesToExecute += 3;
}
void CPU::addWithCarry_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ADC_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    cyclesToExecute += 4;
}
void CPU::addWithCarry_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ADC_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    cyclesToExecute += 4;
}
void CPU::addWithCarry_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ADC_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::addWithCarry_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("ADC_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::addWithCarry_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("ADC_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    addWithCarry(memoryValue);

    cyclesToExecute += 6;
}
void CPU::addWithCarry_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("ADC_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
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
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    cyclesToExecute += 2;
}
void CPU::andWithAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("AND_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::andWithAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::andWithAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("AND_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::andWithAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("AND_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::andWithAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("AND_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    andWithAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::andWithAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("AND_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
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
        uint8_t memoryValue = memory[argument];
        initialValue = memoryValue;

        memoryValue = memoryValue << 1;
        memory[argument] = memoryValue;
        finalValue = memory[argument];
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
    bitset<8> arg(argument);

    //check 7th bit (negative)
    if(arg.test(0) == 1) { flags.negative = 1; } else { flags.negative = 0; }
    //check 6th bit (overflow)
    if(arg.test(1) == 1) { flags.overflow = 1; } else { flags.overflow = 0; }
    //check if arg && accumulator are 0 (zero)
    if(argument && accumulator == 0) { flags.zero = 1; } else { flags.zero = 0; }
}
void CPU::bitTest_ZeroPage() {
    uint8_t argument = retrieveZeroPageInstruction("BIT_ZEROPAGE");
    uint8_t memoryValue = memory[argument];
    bitTest(memoryValue);

    cyclesToExecute += 3;
}
void CPU::bitTest_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("BIT_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
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

    //the actual breakFlag is not set, so we'll reset it after this
    flags.breakFlag = 1;


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
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("CMP_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::compareAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CMP_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::compareAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("CMP_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::compareAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("CMP_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::compareAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("CMP_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    compareAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::compareAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("CMP_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
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
    uint8_t memoryValue = memory[argument];
    compareX(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareX_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CPX_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
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
    uint8_t memoryValue = memory[argument];
    compareY(memoryValue);

    cyclesToExecute += 3;
}
void CPU::compareY_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("CPY_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    compareY(memoryValue);

    cyclesToExecute += 4;
}

void CPU::decrementMemory(uint16_t argument) {
    memory[argument]--;

    if(util.isNegativeByte(memory[argument]) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(memory[argument] == 0) { flags.zero = 1; } else { flags.zero = 0; }
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
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::exclusiveOrAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("EOR_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::exclusiveOrAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("EOR_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::exclusiveOrAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("EOR_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::exclusiveOrAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("EOR_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::exclusiveOrAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("EOR_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::exclusiveOrAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("EOR_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
    exclusiveOrAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::incrementMemory(uint16_t argument) {
    memory[argument]++;

    if(util.isNegativeByte(memory[argument]) == false) { flags.negative = 0; } else { flags.negative = 1; }
    if(memory[argument] == 0) { flags.zero = 1; } else { flags.zero = 0; }
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

    memory[256 + (stackPointer--)] = highByte;
    memory[256 + (stackPointer--)] = lowByte;

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
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDA_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDA_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDA_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::loadAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDA_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::loadAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("LDA_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    loadAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::loadAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("LDA_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
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
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadXIndex_ZeroPageY() {
    uint8_t argument = retrieveZeroPageYInstruction("LDX_ZEROPAGEY");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadXIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDX_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadXIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadXIndex_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("LDX_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
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
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);

    cyclesToExecute += 3;
}
void CPU::loadYIndex_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("LDY_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadYIndex_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("LDY_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    loadYIndex(memoryValue);

    cyclesToExecute += 4;
}
void CPU::loadYIndex_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("LDY_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
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
        uint8_t memoryValue = memory[argument];
        initialValue = memoryValue;

        //TODO: THIS NEEDS TO BE TESTED AGAIN
        memoryValue = memoryValue >> 1;
        memory[argument] = memoryValue;
        finalValue = memory[argument];
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

    uint8_t lowByte = memory[256 + (++stackPointer)];
    uint8_t highByte = memory[256 + (++stackPointer)];

    uint16_t newProgramCounter = util.getWordFromBytes(lowByte, highByte);
    newProgramCounter++;
    programCounter = newProgramCounter;

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
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    cyclesToExecute += 2;
}
void CPU::orWithAccumulator_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("ORA_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    cyclesToExecute += 3;
}
void CPU::orWithAccumulator_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("ORA_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    cyclesToExecute += 4;
}
void CPU::orWithAccumulator_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("ORA_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::orWithAccumulator_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("ORA_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::orWithAccumulator_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("ORA_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    orWithAccumulator(memoryValue);

    cyclesToExecute += 6;
}
void CPU::orWithAccumulator_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("ORA_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
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
        initialValue = memory[argument];
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
        memory[argument] = finalValue;
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
        initialValue = memory[argument];
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
        memory[argument] = finalValue;
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
    memory[argument] = accumulator;
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
    memory[256 + (stackPointer--)] = accumulator;

    //NOTE: PHA AFFECTS NO FLAGS

    cyclesToExecute += 3;
}
void CPU::pullAccumulator() {
    retrieveImpliedInstruction("PLA");

    accumulator = memory[256 + (++stackPointer)];

    if(util.isNegativeByte(accumulator)) { flags.negative = 1; } else { flags.negative = 0; }
    if(accumulator == ZERO) { flags.zero = 1; } else { flags.zero = 0; }

    cyclesToExecute += 4;
}
void CPU::pushProcessorStatus() {
    retrieveImpliedInstruction("PHP");

    //the binary value for the flags need to be pushed onto stack
    uint8_t processorStatus = getProcessorFlagsAsByte();
    memory[256 + (stackPointer--)] = processorStatus;

    //NOTE: PHP AFFECTS NO FLAGS, BUT PLP DOES

    cyclesToExecute += 3;
}
void CPU::pullProcessorStatus() {
    retrieveImpliedInstruction("PLP");

    uint8_t processorStatus = memory[256 + (++stackPointer)];
    setProcessorFlagsFromByte(processorStatus);

    //PLP AFFECTS ALL FLAGS, BUT THEY ARE SET IN setProcessorFlagsFromByte(uint8_t arg)

    cyclesToExecute += 4;
}

void CPU::storeXRegister(uint16_t argument) {
    //NOTE: STX affects no flags
    memory[argument] = xIndex;
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
    memory[argument] = yIndex;
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
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 3;
}
void CPU::subtractWithBorrow_ZeroPageX() {
    uint8_t argument = retrieveZeroPageXInstruction("SBC_ZEROPAGEX");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 4;
}
void CPU::subtractWithBorrow_Absolute() {
    uint16_t argument = retrieveAbsoluteInstruction("SBC_ABSOLUTE");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 4;
}
void CPU::subtractWithBorrow_AbsoluteX() {
    uint16_t argument = retrieveAbsoluteXInstruction("SBC_ABSOLUTEX");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::subtractWithBorrow_AbsoluteY() {
    uint16_t argument = retrieveAbsoluteYInstruction("SBC_ABSOLUTEY");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 5;
    } else {
        cyclesToExecute += 4;
    }
}
void CPU::subtractWithBorrow_IndexedIndirectX() {
    uint16_t argument = retrieveIndexedIndirectXInstruction("SBC_INDEXED_INDIRECTX");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    cyclesToExecute += 6;
}
void CPU::subtractWithBorrow_IndirectIndexedY() {
    uint16_t argument = retrieveIndirectIndexedYInstruction("SBC_INDIRECT_INDEXEDY");
    uint8_t memoryValue = memory[argument];
    subtractWithBorrow(memoryValue);

    if(pageBoundaryCrossed) {
        cyclesToExecute += 6;
    } else {
        cyclesToExecute += 5;
    }
}

void CPU::retrieveImpliedInstruction(std::string instructionString) {
    //DEBUG INFO
    this->instructionString = instructionString;
    this->impliedAddressingMode = true;
    this->accumulatorAddressingMode = false;
    this->addressingMode_8 = false;
    this->addressingMode_16 = false;
}
void CPU::retrieveAccumulatorInstruction(std::string instructionString) {
    //DEBUG INFO
    this->instructionString = instructionString;
    this->impliedAddressingMode = false;
    this->accumulatorAddressingMode = true;
    this->addressingMode_8 = false;
    this->addressingMode_16 = false;
}
uint8_t CPU::retrieveRelativeInstruction(string instructionString) {
    uint8_t argument = memory[programCounter++];

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
    uint8_t argument = memory[programCounter++];

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
    uint8_t argument = memory[programCounter++];

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
    uint8_t argument = memory[programCounter++];
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
    uint8_t argument = memory[programCounter++];
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
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
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
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
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
    uint8_t byteLow = memory[programCounter++];
    uint8_t byteHigh = memory[programCounter++];
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
    uint8_t zeroPageLocation = memory[programCounter++];

    zeroPageLocation += xIndex;

    uint8_t lowByte = memory[zeroPageLocation++];
    uint8_t highByte = memory[zeroPageLocation];

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
    uint8_t zeroPageLocation = memory[programCounter++];

    uint8_t lowByte = memory[zeroPageLocation++];
    uint8_t highByte = memory[zeroPageLocation];

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
    uint8_t indirectByteLow = memory[programCounter++];
    uint8_t indirectByteHigh = memory[programCounter++];
    uint16_t indirectAddress;

    indirectAddress = util.getWordFromBytes(indirectByteLow, indirectByteHigh);
    uint8_t byteLow = memory[indirectAddress++];
    uint8_t byteHigh = memory[indirectAddress];
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

//TODO: THESE NEED TO BE CHANGED TO REFLECT MEMORY LOCATIONS THAT MIRROR OTHER MEMORY LOCATIONS
//TODO: ALSO THESE NEED TO BE USED WHEREVER MEMORY IS BEING AFFECTED BY ANYTHING WHATSOEVER
void CPU::storeByteInMemory(uint8_t byte, uint16_t location) {
    memory[location] = byte;
}
void CPU::storeWordInMemory(uint8_t lowByte, uint8_t highByte, uint16_t location) {
    memory[location++] = lowByte;
    memory[location] = highByte;
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

    uint8_t whatever = processorStatus & 128;

    if((processorStatus & 128) != 0) { flags.negative = 1; } else { flags.negative = 0; }
    if((processorStatus & 64) != 0) { flags.overflow = 1; } else { flags.overflow = 0; }
    //break and ignored are not affected. skip 32 and 16
    if((processorStatus & 8) != 0) { flags.decimal = 1; } else { flags.decimal = 0; }
    if((processorStatus & 4) != 0) { flags.interrupt = 1; } else { flags.interrupt = 0; }
    if((processorStatus & 2) != 0) { flags.zero = 1; } else { flags.zero = 0; }
    if((processorStatus & 1) != 0) { flags.carry = 1; } else { flags.carry = 0; }
}
//
// Created by Christian Curry on 3/4/16.
//

#include "CPU.h"
#include "opcodes.h"
#include "Instructions.h"

using namespace std;

CPU::CPU(RAM* ram) {
    this->ram = ram;

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
    flags.breakFlag = 0;
    flags.decimal = 0;
    flags.interrupt = 0;
    flags.zero = 0;
    flags.carry = 0;
}

void CPU::onPowerUp() {
    setProcessorFlagsFromByte(0x34);
    accumulator = 0x00;
    xIndex = 0x00;
    yIndex = 0x00;

    writeMemoryLocation(0x4017, 0x00);
    writeMemoryLocation(0x4015, 0x00);

    for(uint16_t i = 0x4000; i <= 0x400F; i++) {
        //TODO: dev wiki is unsure about $4010-$4013
        writeMemoryLocation(i, 0x00);
    }

    //TODO: all 15 bits of noise channel LFSR = 0
}

void CPU::onReset() {
    stackPointer -= 0x03;
    setProcessorFlagsFromByte(getProcessorFlagsAsByte() || 0x04);
    writeMemoryLocation(0x4015, 0x00);
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

        //TODO: handle interrupt

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
    opcode = readMemoryLocation(programCounter++);
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

        //RTS+RTI
        case RTS:
            returnFromSubroutine();
            break;
        case RTI:
            returnFromInterrupt();
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

    if(debug) {
        printCPUStatus();
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

//DEFAULT 6502 BEHAVIOR
//void CPU::writeMemoryLocationDefault(uint16_t address, uint8_t value) {
//    memory[address] = value;
//}
//uint8_t CPU::readMemoryLocationDefault(uint16_t address) {
//    return memory[address];
//}
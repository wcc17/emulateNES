#include <CPU.h>
#include "gtest/gtest.h"

//sample tests
//TEST(CPU_Test, test_eq) {
//    EXPECT_EQ(1, 0);
//}
//
//TEST(CPU_Test, test_neq) {
//    EXPECT_NE(1, 0);
//}

static void resetMemory(CPU *cpu) {
    for(int i = 0; i < (65534); i++) {
        cpu->memory[i] = 0x00;
    }
}

static void resetFlags(CPU *cpu) {
    cpu->flags.negative = 0;
    cpu->flags.overflow = 0;
    cpu->flags.ignored = 1;
    cpu->flags.breakFlag = 1;
    cpu->flags.decimal = 0;
    cpu->flags.interrupt = 0;
    cpu->flags.zero = 0;
    cpu->flags.carry = 0;
}

static void resetCPU(CPU *cpu) {
    resetFlags(cpu);
    resetMemory(cpu);

    cpu->accumulator = 0x00;
    cpu->programCounter = 0x0000;
    cpu->stackPointer = 0xff;
    cpu->xIndex = 0x00;
    cpu->yIndex = 0x00;
}

static bool checkFlags(CPU *cpu, int negative, int overflow, int ignored, int breakFlag, int decimal, int interrupt, int zero, int carry) {
    if(cpu->flags.negative == negative
            && cpu->flags.overflow == overflow
            && cpu->flags.ignored == ignored
            && cpu->flags.breakFlag == breakFlag
            && cpu->flags.decimal == decimal
            && cpu->flags.interrupt == interrupt
            && cpu->flags.zero == zero
            && cpu->flags.carry == carry) {
        return true;
    } else {
        return false;
    }
}

//can do these tests seperately because it will be called by all the rest
//and the rest will use the exact same type of value for the argument.
//if they don't their respective test will fail.
TEST(CPU_Test, addWithCarry_noFlags) {
    CPU *cpu = new CPU();
    cpu->accumulator = 0x02;
    uint8_t argument = 0x01;

    //TEST NO FLAGS
    uint8_t oldAccumulatorValue = cpu->accumulator;
    cpu->addWithCarry(argument);

    EXPECT_EQ(argument + oldAccumulatorValue, cpu->accumulator);
    EXPECT_TRUE(checkFlags(cpu, 0, 0, 1, 1, 0, 0, 0, 0));

    resetCPU(cpu);
}
TEST(CPU_Test, addWithCarry_carryFlag) {
    CPU *cpu = new CPU();
    cpu->accumulator = 0xFF;
    cpu->flags.carry = 1;
    uint8_t argument = 0xFF;
    uint8_t oldAccumulatorValue = cpu->accumulator;

    //test that carry flag is 1 when a carry happens
    cpu->addWithCarry(argument);
    uint8_t sum = argument + oldAccumulatorValue + 1;
    EXPECT_EQ(sum, cpu->accumulator);
    EXPECT_TRUE(checkFlags(cpu, 1, 0, 1, 1, 0, 0, 0, 1));

    //now test that carry flag is 0 when a carry doesn't happen.
    cpu->accumulator = 0x00;
    argument = 0x01;
    oldAccumulatorValue = cpu->accumulator;

    cpu->addWithCarry(argument);
    sum = argument + oldAccumulatorValue + 1;
    EXPECT_EQ(sum, cpu->accumulator);
    EXPECT_TRUE(checkFlags(cpu, 0, 0, 1, 1, 0, 0, 0, 0));


}
TEST(CPU_Test, addWithCarryImmediateTest) {
//    CPU *cpu = new CPU();
//
//    uint8_t argument = 0x01;
//    cpu->addWithCarry_Immediate(argument);
//
//    EXPECT_EQ(argument+cpu->accumulator, )
}
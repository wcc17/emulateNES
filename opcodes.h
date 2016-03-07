//
// Created by Christian Curry on 2/22/16.
//

#ifndef EMULATE6502_OPCODES_H
#define EMULATE6502_OPCODES_H

#include <string>

enum OpCodes {
    ADC_IMM = 0x69,
    ADC_ZEROPAGE = 0x65,
    ADC_ZEROPAGEX = 0x75,
    ADC_ABS = 0x6D,
    ADC_ABSX = 0x7D,
    ADC_ABSY = 0x79,
    ADC_INDIRECTX = 0x61,
    ADC_INDIRECTY = 0x71,

    INX = 0XE8,

    LDA_ABS = 0xAD,
    LDA_IMM = 0xA9,

    STA_ABS = 0x8D,

    TAX = 0XAA,
};

#endif //EMULATE6502_OPCODES_H

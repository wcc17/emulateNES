//
// Created by Christian Curry on 2/22/16.
//

#ifndef EMULATE6502_OPCODES_H
#define EMULATE6502_OPCODES_H

#include <string>

enum OpCodes {
    LDA_ABS = 0xAD,
    LDA_IMM = 0xA9,

    STA_ABS = 0x8D,
};

#endif //EMULATE6502_OPCODES_H

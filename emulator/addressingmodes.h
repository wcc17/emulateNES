//
// Created by Christian Curry on 2/23/16.
//

#ifndef EMULATE6502_ADDRESSINGMODES_H
#define EMULATE6502_ADDRESSINGMODES_H

//TODO: ADD THE REST
enum AddressingMode {
    NULL_ADDRESSING_MODE,
    IMPLIED,
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECTX,
    INDIRECTY
};

#endif //EMULATE6502_ADDRESSINGMODES_H

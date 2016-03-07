//
// Created by Christian Curry on 3/5/16.
//

#include "Util.h"

using namespace std;

void Util::printByte(uint8_t byte) {
    cout << "$" << setfill('0') << setw(2) << hex << static_cast<int>(byte);
}

void Util::printWord(uint16_t word) {
    cout << "$" << setfill('0') << setw(4) << hex << static_cast<int>(word);
}

bool Util::isNegativeByte(uint8_t c) {
    bool isNegative = (c >> 0) & 128;
    return isNegative;
}

bool Util::isNegativeWord(uint16_t c) {
    bool isNegative = (c >> 0) & 65536;
    return isNegative;
}

//10 & 8



//128 64  32  16  8   4   2  1
//1   0   1   0   1   0   1  0

//  00001010
//& 00001000
//00000000      -> this should be negative
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

//8 = 1000
//8 & 0x0a (0x0a == 10 == 1010) = 1000. so c & mask don't equal zero, so method will return 1
bool Util::isNegativeByte(uint8_t c) {

//    To get the value of the bit 3 of an uint8_t a, you can use this expression:
//
//    ((a >> 3)  & 0x01)
//    which would be evaluated to 1 if bit 3 is set and 0 if bit 3 is not set.
    bool isNegative = ((c >> 8)) & 128;

    return isNegative;
}

//128 = 10000000
//128 & 0x84 (0x84 == 132 == 10000100) = 10000000. so c & mask don't equal zero. so method will return 1.
bool Util::isNegativeWord(uint16_t c) {

    bool isNegative = ((c >> 1)) & 0x01;
    return isNegative;
}

//10 & 8



//128 64  32  16  8   4   2  1
//1   0   1   0   1   0   1  0

//  00001010
//& 00001000
//00000000      -> this should be negative
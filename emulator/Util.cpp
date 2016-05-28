//
// Created by Christian Curry on 3/5/16.
//

#include <sstream>
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

uint8_t Util::convertTwosComplement(uint8_t argument) {
    argument = 0xFF - argument;
    argument += 1;

    return argument;
}

bool Util::checkLeastSignificantBit(uint8_t c) {
    bool isSet = (c >> 0) & 1;
    return isSet;
}

uint16_t Util::convertStringToWord(string argument) {
    uint16_t x;

    stringstream ss;
    ss << setfill('0') << setw(4) << hex << argument;
    ss >> x;

    return x;
}

uint16_t Util::getWordFromBytes(uint8_t byteLow, uint8_t byteHigh) {

    //This works because:
    //(0x0002 << 8) | 0x01 = 0x0200 | 0x0001 = 0x0201
    uint16_t word = ((uint16_t)byteHigh << 8) | byteLow;

    return word;
}

void Util::printExecutedByteInstruction(string instruction, uint8_t argument) {
    cout << instruction << " ";
    printByte(argument);
    cout << endl;
}

void Util::printExecutedWordInstruction(string instruction, uint16_t argument) {
    cout << instruction << " ";
    printWord(argument);
    cout << endl;
}

void Util::printExecutedAccumulatorInstruction(std::string instruction) {
    cout << instruction << " " << "A" << endl;
}
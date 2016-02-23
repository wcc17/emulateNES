#include <iostream>
#include "Assembler.h"

using namespace std;

int main() {
    Assembler assembler;

    string fileName = "firstprogram.asm";
    assembler.readFile(fileName.c_str());

    return 0;
}
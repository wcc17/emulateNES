LDX #$AA
STX $0000
LDX #$BB
STX $0001

LDX #$01
DEX
JMP there
there:
    LDY #$01
    DEY
    JMP ($0000)
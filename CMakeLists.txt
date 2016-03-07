cmake_minimum_required(VERSION 3.2)
project(emulate6502)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES emulator/main.cpp emulator/opcodes.h emulator/addressingmodes.h emulator/CPU.cpp emulator/CPU.h emulator/Util.cpp emulator/Util.h)
add_executable(emulate6502 ${SOURCE_FILES} emulator/Assembler.cpp emulator/Assembler.h)
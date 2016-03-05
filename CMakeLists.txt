cmake_minimum_required(VERSION 3.2)
project(emulate6502)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp opcodes.h addressingmodes.h CPU.cpp CPU.h Util.cpp Util.h)
add_executable(emulate6502 ${SOURCE_FILES} Assembler.cpp Assembler.h)
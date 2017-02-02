cmake_minimum_required(VERSION 3.2)
project(emulateNES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
        addressingmodes.h
        Assembler.h
        CPU.h
        NES.h
        opcodes.h
        Util.h
        Mapper.h)

set(SOURCE_FILES
        Assembler.cpp
        CPU.cpp
        NES.cpp
        Util.cpp CPUInstructions.h ROM.cpp ROM.h Mapper.h PPU.cpp PPU.h RAM.cpp RAM.h CPUUtil.h)

add_library(emulator STATIC ${SOURCE_FILES} ${HEADER_FILES})
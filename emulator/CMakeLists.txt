cmake_minimum_required(VERSION 3.2)
project(emulate6502)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
        addressingmodes.h
        Assembler.h
        CPU.h
        opcodes.h
        Util.h
        )

set(SOURCE_FILES
        Assembler.cpp
        CPU.cpp
        Util.cpp)

add_library(emulator STATIC ${SOURCE_FILES} ${HEADER_FILES})
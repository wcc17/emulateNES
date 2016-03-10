cmake_minimum_required(VERSION 3.2)
project(emulate6502)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#new shit
set(SOURCE_FILES main.cpp)
add_executable(emulate6502 ${SOURCE_FILES})

include_directories(emulator)

add_subdirectory(emulator)
add_subdirectory(test_emulator)

target_link_libraries(emulate6502 emulator)

#old shit
#set(SOURCE_FILES main.cpp emulator/opcodes.h emulator/addressingmodes.h emulator/CPU.cpp emulator/CPU.h emulator/Util.cpp emulator/Util.h test_emulator/CPUTest.cpp test_emulator/CPUTest.h)
#add_executable(emulate6502 ${SOURCE_FILES} emulator/Assembler.cpp emulator/Assembler.h)
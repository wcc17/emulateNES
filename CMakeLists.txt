cmake_minimum_required(VERSION 3.2)
project(emulateNES)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Emulator.cpp)
set(HEADER_FILES Emulator.h)
add_executable(emulateNES ${SOURCE_FILES} ${HEADER_FILES})

include_directories(emulator)
include_directories(sample_programs)

add_subdirectory(emulator)
add_subdirectory(test_emulator)

find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED >=2.0.0)
#find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
#include_directories(${SDL2_IMAGE_INCLUDE_DIR})

#target_link_libraries(emulateNES emulator ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
target_link_libraries(emulateNES emulator ${SDL2_LIBRARY} )
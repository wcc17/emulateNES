include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runCPUTests
        CPUTest.cpp)

target_link_libraries(runCPUTests gtest gtest_main)
target_link_libraries(runCPUTests emulator)
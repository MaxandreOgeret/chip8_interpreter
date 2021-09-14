# (c) 2021 Maxandre Ogeret
# Licensed under MIT License

cmake_minimum_required(VERSION 3.19)
project(CHIP8)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

##################################
# Download and install GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)
##################################

include_directories(include ${SDL2_INCLUDE_DIRS})

add_library(CHIP8_L
        src/Instructions.cpp
        src/Memory.cpp
        src/Interface.cpp
        src/RomParser.cpp
        src/RegisterManager.cpp
        )

add_executable(CHIP8
        src/main.cpp
        src/Emulator.cpp
        )

target_link_libraries(CHIP8
        CHIP8_L
        ${SDL2_LIBRARIES}
        )

##################################
enable_testing()

add_executable(init
        test/init.cpp
        test/instructions.cpp
        src/Memory.cpp
)

target_link_libraries(init
        CHIP8_L
        ${SDL2_LIBRARIES}
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(init)
##################################

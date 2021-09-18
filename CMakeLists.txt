# (c) 2021 Maxandre Ogeret
# Licensed under MIT License

cmake_minimum_required(VERSION 3.19)
project(CHIP8)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# Download and install SDL2
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.0.16
        )

# Download and install TCLAP
FetchContent_Declare(
        tclap
        GIT_REPOSITORY https://github.com/mirror/tclap.git
        GIT_TAG v1.2.2
        )
FetchContent_GetProperties(tclap)

# Download and install GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(SDL2 tclap GoogleTest)

add_library(CHIP8_L
        src/Instructions.cpp
        src/Memory.cpp
        src/Interface.cpp
        src/RomParser.cpp
        src/RegisterManager.cpp
        src/Configuration.cpp
        )

add_executable(CHIP8
        src/main.cpp
        src/Interpreter.cpp
        )

target_link_libraries(CHIP8
        CHIP8_L
        SDL2
        )

target_include_directories(CHIP8_L
        PRIVATE ${sdl2_SOURCE_DIR}/include
        PUBLIC include
        )

target_include_directories(CHIP8
        PRIVATE ${tclap_SOURCE_DIR}/include
        )

# Building TESTS
enable_testing()

add_executable(TESTS
        test/init.cpp
        test/instructions.cpp
        src/Memory.cpp
        )

target_link_libraries(TESTS
        CHIP8_L
        gtest_main
        SDL2
        )

configure_file(test/cls.ch8
        ${PROJECT_BINARY_DIR}
        COPYONLY
        )

include(GoogleTest)
gtest_discover_tests(TESTS)
##################################

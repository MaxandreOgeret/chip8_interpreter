# (c) 2021 Maxandre Ogeret
# Licensed under MIT License

cmake_minimum_required(VERSION 3.19)
project(CHIP8)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# DSL2
find_package(SDL2 2.0.0 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

# Download and install GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)

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
        ${SDL2_LIBRARIES}
        )

target_include_directories(CHIP8_L
        PRIVATE ${SDL2_INCLUDE_DIRS}
        PUBLIC include
        )

target_include_directories(CHIP8
        PRIVATE ${tclap_SOURCE_DIR}/include
        )

#set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
install(DIRECTORY DESTINATION /usr/bin)
install(TARGETS CHIP8 DESTINATION /usr/bin)

# Building TESTS
#enable_testing()
#
#add_executable(TESTS
#        test/init.cpp
#        test/instructions.cpp
#        src/Memory.cpp
#        )
#
#target_link_libraries(TESTS
#        CHIP8_L
#        gtest_main
#        SDL2
#        )
#
#configure_file(test/cls.ch8
#        ${PROJECT_BINARY_DIR}
#        COPYONLY
#        )
#
#include(GoogleTest)
#gtest_discover_tests(TESTS)
##################################

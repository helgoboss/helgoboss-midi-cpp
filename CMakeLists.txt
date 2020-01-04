# Support CMake version shipping with Ubuntu 18.04 LTS
cmake_minimum_required(VERSION 3.10)

# By default use locally own vcpkg for package searches
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "lib/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif ()

project(helgoboss-midi)

# Enforce pure C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main target
find_path(GSL_INCLUDE_DIR gsl/gsl)
include_directories(include ${GSL_INCLUDE_DIR})
set(SOURCES
    src/Midi14BitCcMessage.cpp
    src/Midi14BitCcMessageParser.cpp
    src/midi-util.cpp
    src/MidiMessage.cpp
    src/MidiParameterNumberMessage.cpp
    src/MidiParameterNumberMessageParser.cpp
    )
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE include)
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
#install(DIRECTORY include/helgoboss-midi DESTINATION include)

# Test target
find_package(Catch2 CONFIG REQUIRED)
add_executable(${PROJECT_NAME}-tests ${SOURCES}
    test/tests.cpp
    test/Midi14BitCcMessageTest.cpp
    test/MidiParameterNumberMessageTest.cpp
    test/Midi14BitCcMessageParserTest.cpp
    test/MidiParameterNumberMessageParserTest.cpp
    )
target_compile_definitions(${PROJECT_NAME}-tests PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)
include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}-tests)
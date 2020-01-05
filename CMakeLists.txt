# Support CMake version shipping with Ubuntu 18.04 LTS
cmake_minimum_required(VERSION 3.10)

# By default use in-repo vcpkg for package searches
set(CMAKE_TOOLCHAIN_FILE "lib/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Must come after toolchain file stuff
project(helgoboss-midi VERSION 0.1.0 LANGUAGES CXX)

# Main target
# Header-only library (= interface library) GSL doesn't offer find_package(), so we need to find its include directory
# via find_path()
find_path(GSL_INCLUDE_DIR gsl/gsl)
add_library(helgoboss-midi STATIC
    src/Midi14BitCcMessage.cpp
    src/Midi14BitCcMessageParser.cpp
    src/midi-util.cpp
    src/MidiMessage.cpp
    src/MidiParameterNumberMessage.cpp
    src/MidiParameterNumberMessageParser.cpp
    )
# Use generator syntax for INTERFACE-scoped includes to support usage of installed library (typical "Modern CMake"
# pattern, see https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)
target_include_directories(helgoboss-midi
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    src
    ${GSL_INCLUDE_DIR}
    )
# We want GSL Expects() to throw instead of aborting in order to enable clients to handle exceptions gracefully
target_compile_definitions(helgoboss-midi PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)
# We want strict C++-17 (as PUBLIC because we use C++-17 nested namespaces in public headers)
target_compile_features(helgoboss-midi PUBLIC cxx_std_17)
set_target_properties(helgoboss-midi PROPERTIES CXX_EXTENSIONS OFF)

# Install
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
write_basic_package_version_file(helgoboss-midi-config-version.cmake COMPATIBILITY SameMajorVersion)
install(DIRECTORY include/helgoboss-midi DESTINATION include)
install(TARGETS helgoboss-midi
    EXPORT helgoboss-midi-config
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
install(EXPORT helgoboss-midi-config
    NAMESPACE helgoboss-midi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/helgoboss-midi)
add_library(helgoboss-midi::helgoboss-midi ALIAS helgoboss-midi)

# Tests
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
  endif ()
endif ()
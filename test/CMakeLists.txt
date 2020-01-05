find_package(Catch2 CONFIG REQUIRED)
include(Catch)
add_executable(helgoboss-midi-tests
    tests.cpp
    Midi14BitCcMessageTest.cpp
    MidiParameterNumberMessageTest.cpp
    Midi14BitCcMessageParserTest.cpp
    MidiParameterNumberMessageParserTest.cpp
    )
target_compile_features(helgoboss-midi-tests PRIVATE cxx_std_17)
set_target_properties(helgoboss-midi-tests PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(helgoboss-midi-tests PRIVATE Catch2::Catch2 helgoboss-midi::helgoboss-midi)
catch_discover_tests(helgoboss-midi-tests)
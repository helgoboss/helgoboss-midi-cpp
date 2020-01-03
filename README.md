# helgoboss-midi

Reusable C++ code for dealing with MIDI messages

## Use

This is a library supposed to be used within a larger C++ application project. Assuming your application project 
uses CMake as well, the easiest way to use it is to put this library in `lib/helgoboss-midi` (e.g. as Git submodule)
and adjust your `CMakeLists.txt` accordingly: 
```cmake
include_directories(lib/helgoboss-midi/include)
add_subdirectory(lib/helgoboss-midi)
```

This offers much flexibility because the library will be compiled from source as part of your larger project. To make 
this work, you also need to provide the dependencies mentioned in the section below.

## Dependencies

This library depends on the following [vcpkg](https://github.com/microsoft/vcpkg) packages:
- `ms-gsl` (for precondition checking)
- `catch2` (for testing)

For convenience, CMake will automatically pick up your vcpkg dependencies if you provide an environment variable
`VCPKG_ROOT` pointing to the vcpkg directory.


## Configure

This library uses [GSL](https://github.com/microsoft/GSL)'s `Expects()` to check preconditions. By default, if a
precondition is not fulfilled, the application quits. If you want to squeeze out the last bit of performance in your
production build, you can switch off precondition checks by defining `GSL_UNENFORCED_ON_CONTRACT_VIOLATION` before 
adding the subdirectory: 

```cmake
add_definitions(-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION)
```

If you want an exception instead, define `GSL_THROW_ON_CONTRACT_VIOLATION`.

## Develop

### Windows

#### Test
```
ctest --build-and-test . build\win-test --build-target helgoboss-midi-tests --build-generator "Visual Studio 15" --test-command ctest
```

#### Build
```
cmake -G "Visual Studio 15" -B build\win
cmake --build build\win --target helgoboss-midi --config "RelWithDebInfo"
```

### Linux

#### Test
```
ctest --build-and-test . build/linux-test --build-target helgoboss-midi-tests --build-generator "Unix Makefiles" --test-command ctest
```

#### Build
```
mkdir -p build/linux
cd build/linux
cmake -G "Unix Makefiles" ../..
cmake --build . --target helgoboss-midi --config "RelWithDebInfo"
```

### OS X

#### Test
```
ctest --build-and-test . build/osx-test --build-target helgoboss-midi-tests --build-generator "Xcode" --test-command ctest
```

#### Build
```
cmake -G "Xcode" -B build\osx
cmake --build build\osx --target helgoboss-midi --config "RelWithDebInfo"
```
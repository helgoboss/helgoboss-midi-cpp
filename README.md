# helgoboss-midi

Reusable C++ code for dealing with MIDI messages

[![Build Status](https://dev.azure.com/benjaminklum/helgoboss-midi/_apis/build/status/helgoboss.helgoboss-midi?branchName=master)](https://dev.azure.com/benjaminklum/helgoboss-midi/_build/latest?definitionId=1&branchName=master)

## Use

This is a library supposed to be used within a larger C++ application project. Assuming your application project 
uses CMake as well, the easiest way to use it is to put this library in `lib/helgoboss-midi` (e.g. as Git submodule)
and adjust your `CMakeLists.txt` accordingly: 
```cmake
add_subdirectory(lib/helgoboss-midi)
find_package(helgoboss-midi 0.1.0 CONFIG REQUIRED)
target_link_libraries(your-target PRIVATE helgoboss-midi::helgoboss-midi)
```


The library depends on certain packages mentioned in [vcpkg.txt](vcpkg.txt). These are 
vcpkg-specific package names because we use [vcpkg](https://github.com/microsoft/vcpkg) as package manager. If you 
also use vcpkg for package management, you need to add all those packages to your application-wide vcpkg package list.
If not, you need to take care of installing those dependencies yourself.

## Develop

The following commands are tailored to Linux but need only minimal adjustments to be executed on OS X or Windows.

### Prepare
```sh
cd lib/vcpkg
./bootstrap-vcpkg.sh
./vcpkg install @../../vcpkg.txt
```

### Build
CMake >= 3.13
```sh
cmake -B build/linux
cmake --build build/linux --config Release
```

CMake < 3.13
```sh
mkdir -p build/linux
cd build/linux
cmake ../..
cmake --build . --config Release
```

### Test
```sh
cd build/linux
ctest
```

### Install
CMake >= 3.15
```sh
cmake --install build/linux --prefix build/linux/test-install
```
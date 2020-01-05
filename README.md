# helgoboss-midi

Reusable C++ code for dealing with MIDI messages

[![Build Status](https://dev.azure.com/benjaminklum/helgoboss-midi/_apis/build/status/helgoboss.helgoboss-midi?branchName=master)](https://dev.azure.com/benjaminklum/helgoboss-midi/_build/latest?definitionId=1&branchName=master)

## Use

This is a library supposed to be used within a larger C++ application project. Assuming your project uses CMake and this
library is already available to CMake, all you need to do is to adjust your `CMakeLists.txt`:

```cmake
target_link_libraries(your-target PRIVATE helgoboss-midi::helgoboss-midi)
```

Here are some solutions to make the library available to CMake.

### Solution 1: vcpkg

This assumes you have your application-wide [vcpkg](https://github.com/microsoft/vcpkg) instance at `lib/vcpkg`.

1. Add https://github.com/helgoboss/helgoboss-vcpkg-overlays as Git submodule at `lib/helgoboss-vcpkg-overlays`
2. Integrate this into your application-wide vcpkg response file (e.g. `vcpkg.txt`) and use vcpkg to install the added 
   package:
    ```
    helgoboss-midi
    --overlay-ports=../helgoboss-vcpkg-overlays/ports
    ```
3. Add the following to your `CMakeLists.txt` *before* `target_link_libraries`
    ```cmake
    find_package(helgoboss-midi 0.1.0 CONFIG REQUIRED)
    ```
    

### Solution 2: Submodule

1. Install the dependencies of this library mentioned in [vcpkg.txt](vcpkg.txt) using whatever packagement management approach you prefer
2. Copy this library to `lib/helgoboss-midi` (e.g. as Git submodule)
3. Add the following to your `CMakeLists.txt` *before* `target_link_libraries`
    ```cmake
    add_subdirectory(lib/helgoboss-midi)
    ```

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
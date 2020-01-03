# helgoboss-midi

## Prepare

1. Install [vcpkg](https://github.com/microsoft/vcpkg)
1. Install the following vcpkg packages:
    - `ms-gsl`
    - `catch2` (for testing)
1. Set environment variable `VCPKG_ROOT`

## Use

Assuming your C++ application project uses CMake as well and you have this library in `lib/helgoboss-midi` (e.g. as
Git submodule), you can make it available to your code like this: 
```cmake
include_directories(lib/helgoboss-midi/include)
add_subdirectory(lib/helgoboss-midi)
```

This library uses [GSL](https://github.com/microsoft/GSL)'s `Expects()` to do check preconditions. By default, if a
precondition is not fulfilled, the application quits. If you want to squeeze out the last bit of performance, you can
switch off precondition checks by defining `GSL_UNENFORCED_ON_CONTRACT_VIOLATION` before adding the subdirectory: 

```cmake
add_definitions(-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION)
```

If you want an exception instead, define `GSL_THROW_ON_CONTRACT_VIOLATION`.

## Develop

### Windows (MSVC 2017)

#### Test
```
ctest --build-and-test . build\msvc-test --build-target helgoboss-midi-tests --build-generator "Visual Studio 15" --test-command ctest
```

#### Build
```
cmake -G "Visual Studio 15" -B build\msvc
cmake --build build\msvc --target helgoboss-midi --config "RelWithDebInfo"
```

### Linux (Make)

#### Test
```
ctest --build-and-test . build/make-test --build-target helgoboss-midi-tests --build-generator "Unix Makefiles" --test-command ctest
```

#### Build
```
mkdir -p build/make
cd build/make
cmake -G "Unix Makefiles" ../..
cmake --build . --target helgoboss-midi --config "RelWithDebInfo"
```
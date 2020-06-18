# derplib

[Documentation](https://derppening.gitlab.io/derplib-cpp/)

Derplib is a collection of utility functions and classes written for C++.

## Getting Started

These instructions will set the project up and link the libraries against your application. Note that currently only 
Linux platforms are officially supported.

### Prerequisites

- [CMake 3.3.0+](https://cmake.org/download/)
- Any C++ compiler on Linux with support for C++11 or newer
    - **NOTE**: Only tested with Clang and GCC

#### Adding this project as a CMake dependency

Add this project in your `CMakeLists.txt`.

```cmake
add_subdirectory(derplib)
```

This will add the following targets to your project:

- `derplib::base`: Collection of self-written classes
- `derplib::container`: Custom data containers
- `derplib::experimental`: Experimental features that are currently under development
- `derplib::stdext`: Functions and classes which extend the functionality of the C++ standard library
    - `derplib::newlib` is now merged into this library

Link these libraries to your project using `target_link_libraries(<your_project> <libraries_to_link>)`

#### Building as Standalone Library

Clone this repository, and configure this project like any other CMake project.

```
git clone https://gitlab.com/Derppening/derplib-cpp.git
cd derplib-cpp
mkdir build
cmake ..
cmake --build .
```

Note that this library currently has APIs dependent on STL, meaning that if this project is compiled against `libc++`, 
it will not link against other projects which are compiled against `libstdc++`, and vice versa. It is therefore 
recommended to always compile this library as a direct dependency of your project.

#### Building tests

Tests are automatically built if this project is built as a standalone library.

If this project is built as a CMake dependency, enable tests by adding the following line before the `add_subdirectory`
command:

```cmake
set(DERPLIB_RUN_TESTS ON)
```

Tests can be run by `make tests` in the root of the `build` directory.

### CMake Configuration

The following CMake variables can be defined to alter the build properties:

- `DERPLIB_BUILD_DOCS`: Whether to build Doxygen documentation (default: ON for standalone)
- `DERPLIB_RUN_TESTS`: Whether to run the bundled tests (default: ON for standalone)
- `DERPLIB_WARN`: Whether to display compiler warnings (default: ON for standalone)

In addition, derplib will read the CMake variable `CMAKE_CXX_STANDARD` from the parent project, and compile a version of 
the library with features matching the given C++ version. To override this setting, set `DERPLIB_CXX_STD_OVERRIDE` to 
any value accepted by `CMAKE_CXX_STANDARD`. Note that this only affect compiled libraries; header-only libraries are 
still affected by the C++ version as defined in the root project.

For standalone libraries, the library is set to compile in C++11 mode by default.

## Versioning

This project uses [SemVer](http://semver.org/) for versioning.

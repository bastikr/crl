# CMake Resource Library

Embed a whole directory tree into a C++ binary. No dynamic memory allocation access is provided from within your C++ code.

Usage in CMakeLists.txt:

```cmake
# Add the resource library
include("PATH_TO_CMAKE_RESOURCE_LIBRARY/resource_library.cmake")
add_resource_library(assets "${ASSETS_BASE_DIR}")

# Use it wherever it's needed
add_executable(example "main.cpp")
target_link_libraries(example PRIVATE assets)
```

This creates a cmake library which contains all the files contained in the provided directory which then can be used from C++ code.

```c++
#include <assets.h>

int main() {
    std::optional<std::span<std::byte>> ok_icon = assets::get("icons/cross.png");
}
```

**Features**
    * No dynamic memory allocations
    * Re-build automatically when content of source directory changes

## Development

Additional features that might be nice to have:
* Static access to files using consteval (compile time error when file does not exist): Demonstration implemented in branch "feature/static_access" - however then everything needs to be in header file which might be okay but a better split between dynamic and static access code is needed so that compilation is possible when only dynamic access is used.
* C interface
* Maybe C++ interfaces < C++20
* gperf for optimal hashes for dynamic get function

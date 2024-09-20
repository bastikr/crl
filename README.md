# CMake Resource Library

Embed a whole directory tree into a C++ binary. Usage in CMakeLists.txt:

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
    // Runtime access (file might not exist)
    std::optional<crl::FileEntry&> ok_icon = assets::get("icons/ok.png");
    std::span<const std::byte> content = ok_icon.content;

    // Static access (compiler checks the file is available)
    crl::FileEntry& ok_icon = assets::icons::ok_png;
    crl::FileEntry& ok_icon = assets::get_static("icons/ok.png");
}
```

TODO: Can the static variant be done with string literals and constexpr?


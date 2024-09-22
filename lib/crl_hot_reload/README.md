# CMake Resource Library Hot Reload

Careful - only use for development:
* **Dynamic memory allocation**
* **Easy to obtain read-after-free situations**

Dynamically reload assets into your C++ program with an interface similar to the CMake Resource Library generated C++ code.

Usage in CMakeLists.txt

```cmake
# Add the CRL Hot Reload library
add_subdirectory("PATH_TO_CRL_HOT_RELOAD_LIBRARY")

# Use it wherever it's needed
add_executable(example "main.cpp")
target_link_libraries(example PRIVATE crl_hot_reload)
```

In this case the base directory can be defined in C++ code.

```
#include <crl_hot_reload.h>

int main() {
    crl::HotReloader assets{"ASSETS/BASE/DIR"}
    std::optional<std::span<std::byte>> ok_icon = assets.get("icons/ok.png");
}
```

Calling the `get` function multiple times will always return the latest file content. However, the file content of the previous `get` call will be freed.

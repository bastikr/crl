#include <assets.h>
#include <crl.h>
#include <iostream>

int main() {
  std::optional<std::span<const std::byte>> f =
      assets::get_file("logs/source/t.txt");
  if (f.has_value()) {
    std::cout << "Found file: "
              << std::string_view{reinterpret_cast<const char *>(
                                      f.value().data()),
                                  f.value().size()}
              << std::endl;
  } else {
    std::cout << "File not found" << std::endl;
  }
}

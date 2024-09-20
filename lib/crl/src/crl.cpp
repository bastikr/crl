#include <crl.h>

namespace crl {

std::optional<std::span<const std::byte>>
get_file(const crl::DirectoryEntry &directory, std::string_view path) {
  const crl::DirectoryEntry *d = &directory;
  while (true) {
    auto pos = path.find('/');
    if (pos == std::string_view::npos) {
      for (const auto &f : d->files) {
        if (f->name == path) {
          return f->data;
        }
      }
      return {};
    }
    std::string_view sub_d_name = path.substr(0, pos);
    bool found_match = false;
    for (const auto &sub_d : d->subdirectories) {
      if (sub_d->name == sub_d_name) {
        path = path.substr(pos + 1);
        d = sub_d;
        found_match = true;
        break;
      }
    }
    if (!found_match) {
      return {};
    }
  }
}

} // namespace crl

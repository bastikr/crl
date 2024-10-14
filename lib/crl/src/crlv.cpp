#include <crl/crlv.h>

#include <crl/crl.h>
#include <crl/utils/strv.h>

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

namespace crl {

std::optional<std::span<const std::byte>>
get_filev(const crl::DirectoryEntry &directory,
          std::span<std::string_view> path) {
    const crl::DirectoryEntry *d = &directory;
    size_t current_pos = 0;
    size_t len = utils::strv::sizev(path);
    while (true) {
        auto pos = utils::strv::findv(path, current_pos, len, '/');
        if (pos == std::string_view::npos) {
            for (const auto &f : d->files) {
                if (utils::strv::equalv(path, current_pos, len, f->name)) {
                    return f->data;
                }
            }
            return {};
        }
        bool found_match = false;
        for (const auto &sub_d : d->subdirectories) {
            if (utils::strv::equalv(path, current_pos, pos - current_pos,
                                    sub_d->name)) {
                len -= (pos + 1 - current_pos);
                current_pos = pos + 1;
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

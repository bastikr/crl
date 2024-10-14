#include <crl/utils/strv.h>

#include <cstddef>
#include <span>
#include <string_view>

namespace crl::utils::strv {

size_t sizev(std::span<std::string_view> strv) {
    size_t n = 0;
    for (const auto &v : strv) {
        n += v.size();
    }
    return n;
}

size_t findv(std::span<std::string_view> strv, size_t start, size_t len,
             char x) {
    size_t pos = 0;
    size_t remaining_len = len;

    for (const auto &s : strv) {
        if (start >= s.size()) {
            start -= s.size();
            pos += s.size();
            continue;
        }

        const size_t d = s.size() - start;
        const size_t process_len = d < remaining_len ? d : remaining_len;
        const auto sub_s = s.substr(start, process_len);
        const size_t p = sub_s.find(x);

        if (p != std::string_view::npos) {
            return pos + start + p;
        }

        remaining_len -= process_len;
        pos += s.size();
        start = 0;

        if (remaining_len == 0) {
            break;
        }
    }

    return std::string_view::npos;
}

bool equalv(std::span<std::string_view> strv, size_t start, size_t len,
            std::string_view x) {
    size_t remaining_len = len;
    size_t pos = 0;

    for (const auto &s : strv) {
        if (start >= s.size()) {
            start -= s.size();
            continue;
        }

        const size_t d = s.size() - start;
        const size_t process_len = d < remaining_len ? d : remaining_len;

        if (pos + process_len > x.size()) {
            return false;
        }

        for (size_t i = 0; i < process_len; ++i) {
            if (s[start + i] != x[pos + i]) {
                return false;
            }
        }

        pos += process_len;
        remaining_len -= process_len;
        start = 0;

        if (remaining_len == 0) {
            break;
        }
    }
    return pos == x.size();
}

} // namespace crl::utils::strv

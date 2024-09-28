#pragma once

// TODO: Remove algorithm include
#include <algorithm>
#include <array>

namespace crl {

template <size_t N> struct StringLiteral {
    // NOLINTNEXTLINE(*-avoid-c-arrays)
    consteval StringLiteral(const char (&x)[N]) {
        static_assert(N > 0);
        std::copy_n(static_cast<const char *>(x), N, value.data());
    }

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    std::array<char, N> value{};
};

} // namespace crl

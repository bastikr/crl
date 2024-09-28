#pragma once

#include <array>

namespace crl {

template <std::size_t N> struct StringLiteral {
    // NOLINTNEXTLINE(*-avoid-c-arrays)
    consteval StringLiteral(const char (&x)[N]) {
        static_assert(N > 0);
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = x[i];
        }
    }

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    std::array<char, N> value{};
};

} // namespace crl

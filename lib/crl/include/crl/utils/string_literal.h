#pragma once

#include <array>
#include <string_view>

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

template <std::size_t N>
bool operator==(const StringLiteral<N> &a, std::string_view b) {
    if (b.size() != N - 1) {
        return false;
    }
    for (int i = 0; i < N - 1; ++i) {
        if (b[i] != a.value[i]) {
            return false;
        }
    }
    return true;
}

} // namespace crl

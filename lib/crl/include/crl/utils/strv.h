#pragma once

#include <span>
#include <string_view>

namespace crl::utils::strv {

size_t sizev(std::span<std::string_view> strv);

size_t findv(std::span<std::string_view> strv, size_t start, size_t len,
             char x);

bool equalv(std::span<std::string_view> strv, size_t start, size_t len,
            std::string_view x);

} // namespace crl::utils::strv

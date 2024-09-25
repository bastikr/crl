#pragma once

#include "crl.h"

namespace crl {

std::optional<std::span<const std::byte>>
get_filev(const crl::DirectoryEntry &directory,
          std::span<std::string_view> path);

} // namespace crl

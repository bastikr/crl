#pragma once

#include <optional>
#include <span>
#include <string_view>

namespace crl {

struct FileEntry {
    std::string_view name;
    std::span<const std::byte> data;
};

struct DirectoryEntry {
    std::string_view name;
    std::span<const DirectoryEntry *const> subdirectories;
    std::span<const FileEntry *const> files;
};

std::optional<std::span<const std::byte>>
get_file(const crl::DirectoryEntry &directory, std::string_view path);

} // namespace crl

#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <vector>

namespace crl {

class HotReloader {
public:
    HotReloader(std::filesystem::path base_dir);

    std::optional<std::span<std::byte>> get(std::string_view relative_path);

private:
    std::filesystem::path m_base_dir;

    std::map<std::filesystem::path, std::vector<std::byte>> m_data;
};

} // namespace crl

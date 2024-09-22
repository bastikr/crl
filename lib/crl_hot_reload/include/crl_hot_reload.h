#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace crl {

class HotReloader {
public:
    HotReloader(std::string_view base_dir);

    std::optional<std::span<std::byte>> get(std::string_view relative_path);

private:
    std::string_view m_base_dir;

    std::map<std::filesystem::path, std::vector<std::byte>> m_data;
};

} // namespace crl

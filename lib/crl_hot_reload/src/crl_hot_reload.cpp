#include <crl_hot_reload.h>

#include <filesystem>
#include <fstream>
#include <utility>

namespace crl {

HotReloader::HotReloader(std::filesystem::path base_dir)
    : m_base_dir(base_dir) {}

std::optional<std::span<std::byte>> HotReloader::get(std::string_view path) {
    auto p = m_base_dir / path;
    if (std::filesystem::is_regular_file(p)) {
        std::ifstream file(p, std::ios::binary);
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            std::size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<std::byte> data(file_size);

            // NOLINTNEXTLINE(*-reinterpret-cast)
            file.read(reinterpret_cast<char *>(data.data()),
                      static_cast<std::streamsize>(file_size));

            m_data[p] = std::move(data);
            return m_data[p];
        }
    }
    return {};
}

std::optional<std::span<std::byte>>
HotReloader::getv(std::span<std::string_view> path) {
    std::string combined_path;
    for (auto p : path) {
        combined_path += p;
    }
    return get(combined_path);
}

} // namespace crl

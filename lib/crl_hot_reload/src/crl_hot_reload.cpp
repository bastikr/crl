#include <crl_hot_reload.h>

#include <filesystem>
#include <fstream>
#include <utility>

namespace crl {

HotReloader::HotReloader(std::filesystem::path base_dir)
    : m_base_dir(base_dir) {}

std::optional<std::span<std::byte>>
HotReloader::get(std::string_view relative_path) {
    auto path = m_base_dir / relative_path;
    if (std::filesystem::is_regular_file(path)) {
        std::ifstream file(path, std::ios::binary);
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            std::size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<std::byte> data(file_size);

            // NOLINTNEXTLINE(*-reinterpret-cast)
            file.read(reinterpret_cast<char *>(data.data()),
                      static_cast<std::streamsize>(file_size));

            m_data[path] = std::move(data);
            return m_data[path];
        }
    }
    return {};
}

} // namespace crl

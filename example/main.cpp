#include <assets.h>

#include <crl/crl.h>
#include <crl/crlv.h>
#include <crl_hot_reload.h>

#include <array>
#include <iostream>
#include <string_view>

static void print_result(std::optional<std::span<const std::byte>> f) {
    if (f.has_value()) {
        std::cout << "Found file: "
                  << std::string_view{reinterpret_cast<const char *>(
                                          f.value().data()),
                                      f.value().size()}
                  << std::endl;
    } else {
        std::cout << "File not found" << std::endl;
    }
}

int main() {
    constexpr std::string_view path = "logs/source/t.txt";
    std::array<std::string_view, 4> pathv{"logs", "/", "source/t", ".txt"};

    // Compile time access
    print_result(assets::get<"logs/source/t.txt">());

    // Virtual directory based
    print_result(assets::get_file(path));
    print_result(assets::get_filev(pathv));

    // gperf perfect hashing access
    print_result(assets::get_file_ph(path));

    // Hot reloader (for development)
    std::filesystem::path f(__FILE__);
    std::string asset_dir = (f.parent_path() / "assets").string();
    crl::HotReloader hot_reloader(asset_dir);
    print_result(hot_reloader.get(path));
    print_result(hot_reloader.getv(pathv));
}

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
    print_result(assets::get_file("logs/source/t.txt"));

    std::array<std::string_view, 4> b{"logs", "/", "source/t", ".txt"};
    print_result(assets::get_filev(b));

    print_result(assets::get_file_ph("logs/source/t.txt"));

    print_result(assets::get<"logs/source/t.txt">());

    std::filesystem::path p(__FILE__);
    std::string asset_dir = p.parent_path() / "assets";
    crl::HotReloader hot_reloader(asset_dir);
    print_result(hot_reloader.get("logs/source/t.txt"));
}

#include <benchmark/benchmark.h>

#include <array>
#include <assets.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

#define CRL_ASSERT(condition)                                               \
    do {                                                                    \
        if (!(condition)) {                                                 \
            std::printf(                                                    \
                "Assertion failed: (%s), function %s, file %s, line %d.\n", \
                #condition, __FUNCTION__, __FILE__, __LINE__);              \
            std::abort();                                                   \
        }                                                                   \
    } while (false)

namespace {

std::string G_PATH;
std::array<std::string_view, 4> G_PATHV;
bool G_EXISTS;

void bench_get_file(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file(G_PATH);
        CRL_ASSERT(r.has_value() == G_EXISTS);
    }
}

void bench_get_filev(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_filev(G_PATHV);
        CRL_ASSERT(r.has_value() == G_EXISTS);
    }
}

void bench_get_file_ph(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_ph(G_PATH);
        CRL_ASSERT(r.has_value() == G_EXISTS);
    }
}

void bench_get_file_strcmp(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_strcmp(G_PATH);
        CRL_ASSERT(r.has_value() == G_EXISTS);
    }
}

BENCHMARK(bench_get_file);
BENCHMARK(bench_get_filev);
BENCHMARK(bench_get_file_ph);
BENCHMARK(bench_get_file_strcmp);

void usage() { printf("Usage: <path> <exists> [gperf args...]\n"); }

} // namespace

int main(int argc, char **argv) {
    if (argc <= 3) {
        usage();
        return 1;
    }
    const std::string_view exists{argv[2]};
    if (exists == "true") {
        G_EXISTS = true;
    } else if (exists == "false") {
        G_EXISTS = false;
    } else {
        usage();
        return 1;
    }
    G_PATH = argv[1];
    const int l = G_PATHV.size() / 4;
    std::get<0>(G_PATHV) = std::string_view(G_PATH).substr(0, l);
    std::get<1>(G_PATHV) = std::string_view(G_PATH).substr(l, l);
    std::get<2>(G_PATHV) = std::string_view(G_PATH).substr(l + l, l);
    std::get<3>(G_PATHV) = std::string_view(G_PATH).substr(l + l + l);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

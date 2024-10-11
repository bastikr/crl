#include <benchmark/benchmark.h>

#include <assets.h>
#include <cstdio>
#include <string>
#include <string_view>

static std::string g_PATH;
static std::array<std::string_view, 4> g_PATHV;
static bool g_EXISTS;

static void bench_get_file(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file(g_PATH);
        assert(r.has_value() == g_EXISTS);
    }
}

static void bench_get_filev(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_filev(g_PATHV);
        assert(r.has_value() == g_EXISTS);
    }
}

static void bench_get_file_ph(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_ph(g_PATH);
        assert(r.has_value() == g_EXISTS);
    }
}

static void bench_get_file_strcmp(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_strcmp(g_PATH);
        assert(r.has_value() == g_EXISTS);
    }
}

BENCHMARK(bench_get_file);
BENCHMARK(bench_get_filev);
BENCHMARK(bench_get_file_ph);
BENCHMARK(bench_get_file_strcmp);

void usage() { printf("Usage: <path> <exists> [gperf args...]\n"); }

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
        return 1;
    }
    std::string exists{argv[2]};
    if (exists == "true") {
        g_EXISTS = true;
    } else if (exists == "false") {
        g_EXISTS = false;
    } else {
        usage();
        return 1;
    }
    g_PATH = argv[1];
    int l = g_PATHV.size() / 4;
    std::get<0>(g_PATHV) = std::string_view(g_PATH).substr(0, l);
    std::get<1>(g_PATHV) = std::string_view(g_PATH).substr(l, l);
    std::get<2>(g_PATHV) = std::string_view(g_PATH).substr(l + l, l);
    std::get<3>(g_PATHV) = std::string_view(g_PATH).substr(l + l + l);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

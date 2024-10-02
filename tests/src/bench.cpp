#include <benchmark/benchmark.h>

#include <assets.h>

static void bench_get_file(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file("logs/source/t.txt");
        assert(r.has_value());
    }
}

static void bench_get_filev(benchmark::State &state) {
    std::array<std::string_view, 4> b{"logs", "/", "source/t", ".txt"};
    for (auto _ : state) {
        auto r = assets::get_filev(b);
        assert(r.has_value());
    }
}

static void bench_get_file_static(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get<"logs/source/t.txt">();
        assert(r.has_value());
    }
}

static void bench_get_file_ph(benchmark::State &state) {
    std::array<std::string_view, 4> b{"logs", "/", "source/t", ".txt"};
    for (auto _ : state) {
        auto r = assets::get_file_ph("logs/source/t.txt");
        assert(r.has_value());
    }
}

BENCHMARK(bench_get_file);
BENCHMARK(bench_get_filev);
BENCHMARK(bench_get_file_static);
BENCHMARK(bench_get_file_ph);

BENCHMARK_MAIN();

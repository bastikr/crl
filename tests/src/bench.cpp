#include <benchmark/benchmark.h>

#include <assets.h>

static void bench_get_file_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file("logs/source/t.txt");
        assert(r.has_value());
    }
}

static void bench_get_file_not_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file("logs/source/t.txd");
        assert(!r.has_value());
    }
}

static void bench_get_filev_found(benchmark::State &state) {
    std::array<std::string_view, 4> b{"logs", "/", "source/t", ".txt"};
    for (auto _ : state) {
        auto r = assets::get_filev(b);
        assert(r.has_value());
    }
}

static void bench_get_filev_not_found(benchmark::State &state) {
    std::array<std::string_view, 4> b{"logs", "/", "source/t", ".txd"};
    for (auto _ : state) {
        auto r = assets::get_filev(b);
        assert(!r.has_value());
    }
}

static void bench_get_file_static(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get<"logs/source/t.txt">();
        assert(r.size() > 1);
    }
}

static void bench_get_file_ph_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_ph("logs/source/t.txt");
        assert(r.has_value());
    }
}

static void bench_get_file_ph_not_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_ph("logs/source/t.txd");
        assert(!r.has_value());
    }
}

static void bench_get_file_strcmp_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_strcmp("logs/source/t.txt");
        assert(r.has_value());
    }
}

static void bench_get_file_strcmp_not_found(benchmark::State &state) {
    for (auto _ : state) {
        auto r = assets::get_file_strcmp("logs/source/t.txd");
        assert(!r.has_value());
    }
}

BENCHMARK(bench_get_file_static);

BENCHMARK(bench_get_file_found);
BENCHMARK(bench_get_file_not_found);
BENCHMARK(bench_get_filev_found);
BENCHMARK(bench_get_filev_not_found);
BENCHMARK(bench_get_file_ph_found);
BENCHMARK(bench_get_file_ph_not_found);
BENCHMARK(bench_get_file_strcmp_found);
BENCHMARK(bench_get_file_strcmp_not_found);

BENCHMARK_MAIN();

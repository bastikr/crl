import os
import subprocess
import shutil
import sys
import json
from pathlib import Path
import argparse
import datetime
import itertools
from dataclasses import dataclass

# Define the paths
TESTSET_GENERATOR = Path("./generate_testset.py")

# Define CMake and Build Configurations
CMAKE_TEMPLATE = """
cmake_minimum_required(VERSION 3.17)
project(benchmark_project C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("{CRL_DIR}" crl)
find_package(benchmark REQUIRED PATHS "../google-benchmark/build")

add_resource_library(assets "{TESTSET_DIR}" GPERF)

add_executable(crl_benchmark "{CRL_DIR}/tests/src/bench.cpp")
target_link_libraries(crl_benchmark
    PRIVATE
        benchmark::benchmark
        assets
)
"""


@dataclass
class BenchmarkConfig:
    num_files_range: list[int]
    dir_depth_range: list[int]
    crl_dir: Path
    benchmark_base_dir: Path
    timestamp: str


def parse_arguments() -> BenchmarkConfig:
    parser = argparse.ArgumentParser(
        description="Benchmark CMake Resource Library with generated test sets."
    )
    parser.add_argument(
        "--num-files",
        type=str,
        required=True,
        help="Number of files to generate in the test set (single value or range in the form start:end:step).",
    )
    parser.add_argument(
        "--dir-depth",
        type=str,
        required=True,
        help="Directory depth for the generated test set (single value or range in the form start:end:step).",
    )
    parser.add_argument(
        "--crl-dir",
        type=Path,
        required=True,
        help="Path to the CRL directory to link to the benchmark.",
    )
    parser.add_argument(
        "--benchmark-base-dir",
        type=Path,
        required=True,
        help="Base directory for benchmark projects, test sets, and results.",
    )
    args = parser.parse_args()

    num_files_range = parse_range(args.num_files)
    dir_depth_range = parse_range(args.dir_depth)

    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")

    return BenchmarkConfig(
        num_files_range=num_files_range,
        dir_depth_range=dir_depth_range,
        crl_dir=args.crl_dir.resolve(),
        benchmark_base_dir=args.benchmark_base_dir.resolve(),
        timestamp=timestamp,
    )


def parse_range(value: str) -> list[int]:
    return [int(x) for x in value.split(",")]


def setup_testset(num_files: int, dir_depth: int, testsets_dir: Path):
    if testsets_dir.exists():
        shutil.rmtree(testsets_dir)
    testsets_dir.mkdir(parents=True)
    subprocess.run(
        [
            sys.executable,
            TESTSET_GENERATOR,
            "--num-files",
            str(num_files),
            "--dir-depth",
            str(dir_depth),
            "--output-dir",
            str(testsets_dir),
        ],
        check=True,
    )


def setup_google_benchmark(base_dir: Path):
    gbench_dir = base_dir / "google-benchmark"
    if not gbench_dir.exists():
        gbench_dir.mkdir(parents=True)
        subprocess.run(
            [
                "git",
                "clone",
                "https://github.com/google/benchmark.git",
                str(gbench_dir),
            ],
            check=True,
        )
        subprocess.run(
            [
                "cmake",
                "-S",
                str(gbench_dir),
                "-B",
                str(gbench_dir / "build"),
                "-DCMAKE_BUILD_TYPE=Release",
                "-DBENCHMARK_ENABLE_GTEST_TESTS=OFF",
            ],
            check=True,
        )
        subprocess.run(
            ["cmake", "--build", str(gbench_dir / "build"), "--target", "benchmark"],
            check=True,
        )


def get_sorted_files(directory: Path) -> list[str]:
    all_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            full_path = os.path.join(root, file)
            relative_path = os.path.relpath(full_path, directory)
            all_files.append(relative_path)
    return sorted(all_files)


def select_equally_spaced_paths(paths: list[str], n: int):
    if n <= 0:
        return []
    step = len(paths) / n
    return [(int(i * step), paths[int(i * step)]) for i in range(n)]


def change_last_character(x: str) -> str:
    c = "x"
    if x[-1] == c:
        c = "y"
    return x[:-1] + c


def setup_cmake_project(project_dir: Path, testset_dir: Path, crl_dir: Path):
    if project_dir.exists():
        shutil.rmtree(project_dir)
    project_dir.mkdir(parents=True)

    with open(project_dir / "CMakeLists.txt", "w") as f:
        f.write(
            CMAKE_TEMPLATE.format(
                **{"CRL_DIR": str(crl_dir), "TESTSET_DIR": str(testset_dir)}
            )
        )


def configure_cmake_project(project_path: Path):
    build_path = project_path / "build"
    if build_path.exists():
        shutil.rmtree(build_path)
    build_path.mkdir()

    subprocess.run(
        ["cmake", "-DCMAKE_BUILD_TYPE=Release", ".."], cwd=build_path, check=True
    )


def calculate_testset_stats(base_dir: Path):
    total_size = 0
    total_path_length = 0

    for root, dirs, files in os.walk(base_dir):
        for file in files:
            file_path = os.path.join(root, file)

            total_size += os.path.getsize(file_path)

            relative_path = os.path.relpath(file_path, base_dir)
            total_path_length += len(relative_path)
    return {
        "testset_content_size": total_size,
        "testset_total_path_length": total_path_length,
    }


def get_generated_source_size(project_dir: Path) -> int:
    file_path = project_dir / "build/assets/src/assets.cpp"
    return os.path.getsize(file_path)


def get_generated_header_size(project_dir: Path) -> int:
    file_path = project_dir / "build/assets/include/assets.h"
    return os.path.getsize(file_path)


def get_asset_library_size(project_dir: Path) -> int:
    file_path = project_dir / "build/libassets.a"
    return os.path.getsize(file_path)


def build_cmake_project(project_path: Path):
    build_path = project_path / "build"
    subprocess.run(["cmake", "--build", "."], cwd=build_path, check=True)


def run_benchmark(project_path: Path, query_path: str, exists: bool) -> str:
    build_path = project_path / "build"
    if exists:
        exists_str = "true"
    else:
        exists_str = "false"
    result = subprocess.run(
        ["./crl_benchmark", query_path, exists_str, "--benchmark_format=json"],
        cwd=build_path,
        capture_output=True,
        text=True,
        check=True,
    )
    return result.stdout


def run_benchmarks(config: BenchmarkConfig):
    setup_google_benchmark(config.benchmark_base_dir)

    for num_files, dir_depth in itertools.product(
        config.num_files_range, config.dir_depth_range
    ):
        testset_dir = (
            config.benchmark_base_dir / f"testsets_num{num_files}_depth{dir_depth}"
        )
        project_dir = (
            config.benchmark_base_dir / f"projects_num{num_files}_depth{dir_depth}"
        )
        results_file = (
            config.benchmark_base_dir
            / f"results_{config.timestamp}_num{num_files}_depth{dir_depth}.json"
        )

        setup_testset(num_files, dir_depth, testset_dir)
        testset_params = calculate_testset_stats(testset_dir)
        testset_params["number_of_files"] = num_files

        if not project_dir.exists():
            project_dir.mkdir(parents=True)

        setup_cmake_project(project_dir, testset_dir, config.crl_dir)

        gen_code_params = {}
        library_params = {}

        try:
            configure_cmake_project(project_dir)
            build_cmake_project(project_dir)
            gen_code_params["source_size"] = get_generated_source_size(project_dir)
            gen_code_params["header_size"] = get_generated_header_size(project_dir)
            library_params["size"] = get_asset_library_size(project_dir)
        except subprocess.CalledProcessError as e:
            print(f"Error while building for {testset_dir}: {e}")
            break

        testfiles = get_sorted_files(testset_dir)
        query_paths = select_equally_spaced_paths(testfiles, 5)

        runs = []

        for query_path_index, query_path in query_paths:
            run_params = {
                "query_path": query_path,
                "query_path_index": query_path_index,
            }
            try:
                exists = True
                output = run_benchmark(project_dir, query_path, exists)
                runs.append(
                    {**run_params, "exists": exists, "benchmark": json.loads(output)}
                )

                exists = False
                output = run_benchmark(
                    project_dir, change_last_character(query_path), exists
                )
                runs.append(
                    {**run_params, "exists": exists, "benchmark": json.loads(output)}
                )
            except subprocess.CalledProcessError as e:
                print(f"Error while running benchmark for {testset_dir}: {e}")
                break

        with open(results_file, "w") as f:
            json.dump(
                {
                    "timestamp": config.timestamp,
                    "testset": testset_params,
                    "generated": gen_code_params,
                    "library": library_params,
                    "runs": runs,
                },
                f,
            )

        print(f"Result writtent to {results_file}")


if __name__ == "__main__":
    config = parse_arguments()
    run_benchmarks(config)

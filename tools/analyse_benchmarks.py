from pathlib import Path
import argparse
from dataclasses import dataclass, asdict
import json
from glob import glob
import os.path
import datetime
import matplotlib.pyplot as pyplot
import seaborn
import pandas

@dataclass
class Cli:
    benchmark_base_dir: Path

@dataclass
class DataRecord:
    timestamp: datetime
    testset_number_of_files: int
    query_type: str
    query_exists: bool
    query_path: str
    query_path_index: int
    benchmark_cpu_time_ns: float
    benchmark_real_time_ns: float
    benchmark_iterations: int


def parse_arguments():
    parser = argparse.ArgumentParser(description="Analyze CMake Resource Library benchmark results.")
    parser.add_argument("--benchmark-base-dir", type=Path, required=True, help="Base directory for benchmark projects, test sets, and results.")
    args = parser.parse_args()
    return Cli(
        benchmark_base_dir=args.benchmark_base_dir
    )

def load_data(base_dir: Path):
    data = []
    for path in glob(os.path.join(base_dir, "results_*.json")):
        with open(path, 'r') as f:
            content = json.load(f)
            data.append(content)
    return data

def convert_json_to_data_records(json_data):
    records = []
    for suite in json_data:
        timestamp = datetime.datetime.strptime(suite["timestamp"], "%Y%m%d_%H%M%S")
        for run in suite["runs"]:
            for b in run["benchmark"]["benchmarks"]:
                assert b["time_unit"] == "ns"
                records.append(DataRecord(
                    timestamp = timestamp,
                    testset_number_of_files = suite["testset"]["number_of_files"],
                    query_path = run["query_path"],
                    query_path_index = run["query_path_index"],
                    query_exists = run["exists"],
                    query_type = b["name"],
                    benchmark_real_time_ns = b["real_time"],
                    benchmark_cpu_time_ns = b["cpu_time"],
                    benchmark_iterations=b["iterations"]
                ))
    return records

def convert_data_records_to_pandas(data_records):
    return pandas.DataFrame([asdict(record) for record in data_records])

def plot_benchmark(df: pandas.DataFrame):
    # marker_dict = {True: 'o', False: 's'}
    # df['marker'] = df['query_exists'].map(marker_dict)
    worst_benchmarks = df.groupby(['testset_number_of_files', 'query_type'], as_index=False).agg({
        'benchmark_cpu_time_ns': 'max'
    })
    
    seaborn.set_palette("Set2")
    
    pyplot.figure(figsize=(10, 6))
    # for marker, data in df.groupby('marker'):
    seaborn.scatterplot(
        data=worst_benchmarks,
        x="testset_number_of_files",
        y="benchmark_cpu_time_ns",
        hue="query_type",
        s=100,
        edgecolor="black"
    )

    pyplot.xscale('log')
    pyplot.yscale('log')

    pyplot.xlabel("Test Set Number of Files")
    pyplot.ylabel("Benchmark CPU Time (ns)")
    pyplot.title("Test Set Files vs. CPU Time with Query Type and Existence Markers")
    pyplot.legend(title="Query Type")

    pyplot.show()

if __name__ == "__main__":
    args = parse_arguments()
    json_data = load_data(args.benchmark_base_dir)
    data_records = convert_json_to_data_records(json_data)
    df = convert_data_records_to_pandas(data_records)
    plot_benchmark(df)

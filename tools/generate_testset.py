#!/usr/bin/env python3

import os
import argparse
import random
import string
import math
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Cli:
    output_dir: Path
    number_of_files: int
    content_size: int
    path_length: int
    files_per_dir: int
    dir_depth: int
    distribution: str


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Generate a test dataset with configurable directory and file characteristics."
    )
    parser.add_argument(
        "--output-dir",
        type=str,
        required=True,
        help="Root directory to generate the dataset.",
    )
    parser.add_argument(
        "--num-files",
        type=int,
        required=True,
        help="Total number of files to generate.",
    )
    parser.add_argument(
        "--files-per-dir", type=int, default=10, help="Number of files per directory."
    )
    parser.add_argument(
        "--distribution",
        type=str,
        choices=["balanced", "unbalanced"],
        default="balanced",
        help="Distribution of files across directories.",
    )
    parser.add_argument(
        "--content-size",
        type=int,
        default="10",
        help="Size of each file's content in bytes.",
    )
    parser.add_argument(
        "--dir-depth", type=int, default=3, help="Maximum depth of the directory tree."
    )
    parser.add_argument(
        "--path-length",
        type=int,
        default=100,
        help="Desired average path length in characters.",
    )
    args = parser.parse_args()
    return Cli(
        output_dir=args.output_dir,
        number_of_files=args.num_files,
        content_size=args.content_size,
        path_length=args.path_length,
        files_per_dir=args.files_per_dir,
        dir_depth=args.dir_depth,
        distribution=args.distribution,
    )


def random_string(min_length=5, max_length=15):
    length = random.randint(min_length, max_length)
    return "".join(random.choices(string.ascii_letters + string.digits, k=length))


def create_file(path: Path, size: int):
    with open(path, "wb") as f:
        f.write(os.urandom(size))


def distribute_files_balanced(num_files, files_per_dir):
    num_dirs = math.ceil(num_files / files_per_dir)
    distribution = [files_per_dir] * num_dirs
    remaining = num_files - files_per_dir * num_dirs
    for i in range(remaining):
        distribution[i] += 1
    return distribution


def distribute_files_unbalanced(num_files, files_per_dir):
    num_dirs = math.ceil(num_files / files_per_dir)
    distribution = []
    remaining = num_files
    for _ in range(num_dirs):
        if remaining <= 0:
            break
        files_in_dir = random.randint(1, min(files_per_dir, remaining))
        distribution.append(files_in_dir)
        remaining -= files_in_dir
    # If there are remaining files, assign them randomly
    while remaining > 0:
        idx = random.randint(0, len(distribution) - 1)
        if distribution[idx] < files_per_dir:
            distribution[idx] += 1
            remaining -= 1
    return distribution


def adjust_name_lengths(
    target_path_length, current_depth, max_depth, current_path_length, path_parts
):
    remaining_length = target_path_length - current_path_length
    remaining_dirs = max_depth - current_depth
    if remaining_dirs <= 0:
        file_name_length = max(5, remaining_length - 10)  # Reserve some for file name
    else:
        avg_dir_length = remaining_length // (remaining_dirs + 1)
        file_name_length = max(5, avg_dir_length)
    return file_name_length


def main():
    args = parse_arguments()

    path_length = args.path_length

    os.makedirs(args.output_dir, exist_ok=True)

    if args.distribution == "balanced":
        files_distribution = distribute_files_balanced(
            args.number_of_files, args.files_per_dir
        )
    else:
        files_distribution = distribute_files_unbalanced(
            args.number_of_files, args.files_per_dir
        )

    total_dirs = len(files_distribution)
    print(
        f"Generating {args.number_of_files} files across {total_dirs} directories with a maximum depth of {args.dir_depth}."
    )

    # Generate directories and files
    for dir_idx, num_files_in_dir in enumerate(files_distribution):
        current_path = args.output_dir
        current_depth = 0
        current_path_length = len(os.path.abspath(current_path))
        path_parts = []
        while current_depth < args.dir_depth:
            file_name_length = adjust_name_lengths(
                path_length,
                current_depth,
                args.dir_depth,
                current_path_length,
                path_parts,
            )
            dir_name = random_string(min_length=3, max_length=file_name_length)
            path_parts.append(dir_name)
            current_path = os.path.join(current_path, dir_name)
            current_path_length += len(dir_name) + 1  # +1 for the os.sep
            current_depth += 1
            if current_path_length >= path_length - 50:
                break
        os.makedirs(current_path, exist_ok=True)

        for file_idx in range(num_files_in_dir):
            file_name_length = max(5, min(15, path_length // 10))
            file_name = (
                f"file_{random_string(min_length=3, max_length=file_name_length)}.bin"
            )
            file_path = os.path.join(current_path, file_name)
            if len(os.path.abspath(file_path)) > path_length:
                excess = len(os.path.abspath(file_path)) - path_length
                if (
                    excess > 0 and len(file_name) > excess + 4
                ):  # 4 for 'file_' and '.bin'
                    new_length = len(file_name) - excess
                    file_name = (
                        f"file_{random_string(min_length=3, max_length=new_length)}.bin"
                    )
                    file_path = os.path.join(current_path, file_name)
            create_file(file_path, args.content_size)

    print(f"Dataset generation complete. Files are located in '{args.output_dir}'.")


if __name__ == "__main__":
    main()

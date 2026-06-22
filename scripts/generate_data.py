#!/usr/bin/env python3
import argparse
import csv
import math
import random
from pathlib import Path


def generate_sin_data(path: Path, seed: int, count: int) -> None:
    rng = random.Random(seed)
    with path.open("w", newline="") as handle:
        writer = csv.writer(handle, lineterminator="\n")
        writer.writerow(["x", "y"])
        for index in range(count):
            x = index * 10.0 / (count - 1)
            y = math.sin(x) + rng.gauss(0.0, 0.1)
            writer.writerow([f"{x:.8f}", f"{y:.8f}"])


def generate_xnor_data(path: Path) -> None:
    rows = [
        (0.0, 0.0, 1.0),
        (0.0, 1.0, 0.0),
        (1.0, 0.0, 0.0),
        (1.0, 1.0, 1.0),
    ]
    with path.open("w", newline="") as handle:
        writer = csv.writer(handle, lineterminator="\n")
        writer.writerow(["x1", "x2", "y"])
        writer.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate example datasets.")
    parser.add_argument("--output-dir", default="data", help="Directory for CSV files.")
    parser.add_argument("--seed", type=int, default=42, help="Random seed for noisy data.")
    parser.add_argument(
        "--sin-count",
        type=int,
        default=201,
        help="Number of samples to generate for sin_data.csv.",
    )
    args = parser.parse_args()
    if args.sin_count < 2:
        parser.error("--sin-count must be at least 2")

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    generate_sin_data(output_dir / "sin_data.csv", args.seed, args.sin_count)
    generate_xnor_data(output_dir / "xnor_data.csv")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

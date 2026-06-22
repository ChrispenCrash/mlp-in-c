#!/usr/bin/env python3
import argparse
import csv
import random
import struct
from pathlib import Path


def read_csv(path: Path):
    with path.open(newline="") as handle:
        reader = csv.DictReader(handle)
        if reader.fieldnames is None:
            raise ValueError(f"{path} does not contain a header row")

        rows = []
        for row in reader:
            rows.append({name: float(row[name]) for name in reader.fieldnames})

    if not rows:
        raise ValueError(f"{path} does not contain any data rows")
    return reader.fieldnames, rows


def write_matrix(path: Path, rows, col_count: int) -> None:
    row_count = len(rows)

    with path.open("wb") as handle:
        handle.write(struct.pack("=ii", row_count, col_count))
        for row in rows:
            if len(row) != col_count:
                raise ValueError("all rows must have the same number of columns")
            handle.write(struct.pack(f"={col_count}d", *row))


def split_rows(rows, test_ratio: float, seed: int):
    shuffled = list(rows)
    random.Random(seed).shuffle(shuffled)

    test_count = round(len(shuffled) * test_ratio)
    if test_ratio > 0.0 and len(shuffled) > 1:
        test_count = max(1, min(len(shuffled) - 1, test_count))

    return shuffled[test_count:], shuffled[:test_count]


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Prepare a numeric CSV as binary matrices for the C MLP."
    )
    parser.add_argument("--input", required=True, help="Input CSV with a header row.")
    parser.add_argument(
        "--output-dir", required=True, help="Directory for prepared .bin files."
    )
    parser.add_argument("--target", required=True, help="Target column name.")
    parser.add_argument(
        "--test-ratio",
        type=float,
        default=0.2,
        help="Fraction of rows to reserve for test data.",
    )
    parser.add_argument("--seed", type=int, default=42, help="Shuffle seed.")
    args = parser.parse_args()

    if not 0.0 <= args.test_ratio < 1.0:
        raise ValueError("--test-ratio must be in [0.0, 1.0)")

    fieldnames, rows = read_csv(Path(args.input))
    if args.target not in fieldnames:
        raise ValueError(f"target column '{args.target}' not found in {args.input}")

    features = [name for name in fieldnames if name != args.target]
    if not features:
        raise ValueError("at least one feature column is required")

    train_rows, test_rows = split_rows(rows, args.test_ratio, args.seed)

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    write_matrix(
        output_dir / "x_train.bin",
        [[row[name] for name in features] for row in train_rows],
        len(features),
    )
    write_matrix(
        output_dir / "y_train.bin", [[row[args.target]] for row in train_rows], 1
    )
    write_matrix(
        output_dir / "x_test.bin",
        [[row[name] for name in features] for row in test_rows],
        len(features),
    )
    write_matrix(
        output_dir / "y_test.bin", [[row[args.target]] for row in test_rows], 1
    )

    with (output_dir / "metadata.txt").open("w") as handle:
        handle.write(f"source={args.input}\n")
        handle.write(f"features={','.join(features)}\n")
        handle.write(f"target={args.target}\n")
        handle.write(f"train_rows={len(train_rows)}\n")
        handle.write(f"test_rows={len(test_rows)}\n")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

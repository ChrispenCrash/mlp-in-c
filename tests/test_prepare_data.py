import csv
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


def read_matrix(path: Path):
    with path.open("rb") as handle:
        rows, cols = struct.unpack("ii", handle.read(8))
        values = struct.unpack(f"{rows * cols}d", handle.read(rows * cols * 8))
    return rows, cols, values


def main() -> int:
    repo = Path(__file__).resolve().parents[1]

    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        source = tmp / "sample.csv"
        output = tmp / "prepared"
        output.mkdir()

        with source.open("w", newline="") as handle:
            writer = csv.writer(handle)
            writer.writerow(["x1", "x2", "y"])
            writer.writerow([0.0, 0.0, 1.0])
            writer.writerow([0.0, 1.0, 0.0])
            writer.writerow([1.0, 0.0, 0.0])
            writer.writerow([1.0, 1.0, 1.0])

        subprocess.run(
            [
                sys.executable,
                str(repo / "scripts" / "prepare_data.py"),
                "--input",
                str(source),
                "--output-dir",
                str(output),
                "--target",
                "y",
                "--test-ratio",
                "0.25",
                "--seed",
                "1",
            ],
            check=True,
        )

        x_train = read_matrix(output / "x_train.bin")
        y_train = read_matrix(output / "y_train.bin")
        x_test = read_matrix(output / "x_test.bin")
        y_test = read_matrix(output / "y_test.bin")

        assert x_train[:2] == (3, 2)
        assert y_train[:2] == (3, 1)
        assert x_test[:2] == (1, 2)
        assert y_test[:2] == (1, 1)

        metadata = (output / "metadata.txt").read_text()
        assert "features=x1,x2" in metadata
        assert "target=y" in metadata

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

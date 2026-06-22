import subprocess
import sys
import tempfile
from pathlib import Path


def main() -> int:
    repo = Path(__file__).resolve().parents[1]

    with tempfile.TemporaryDirectory() as tmpdir:
        result = subprocess.run(
            [
                sys.executable,
                str(repo / "scripts" / "generate_data.py"),
                "--output-dir",
                tmpdir,
                "--sin-count",
                "1",
            ],
            capture_output=True,
            text=True,
        )

    assert result.returncode != 0
    assert "--sin-count must be at least 2" in result.stderr
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

## MLP in C

This project is a small neural-network playground in C. Data preparation is
handled by Python so the C code can focus on matrix operations and the MLP
implementation.

Prepared matrix files use a simple binary format:

```text
int rows                    # native C int, 4 bytes on supported builds
int cols                    # native C int, 4 bytes on supported builds
double values[rows * cols]  # native C double, row-major order
```

The binary files are intentionally simple and are meant to be prepared and
consumed on the same platform. CSV remains the portable, human-readable format
at the edges of the workflow.

## Usage

Generate the example CSV datasets:

```bash
python3 scripts/generate_data.py
```

Prepare a dataset for C:

```bash
python3 scripts/prepare_data.py \
  --input data/xnor_data.csv \
  --output-dir data/prepared \
  --target y
```

Compile the C program:

```bash
make
```

Run the multilayer perceptron scaffold on prepared matrices:

```bash
./mlp data/prepared/x_train.bin data/prepared/y_train.bin
```

Run the tests:

```bash
make test
```

To remove the executables, use:

```bash
make clean
```
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../mat.h"

#define ASSERT_TRUE(condition, message)          \
    do {                                        \
        if (!(condition)) {                     \
            fprintf(stderr, "%s\n", message);  \
            return 1;                           \
        }                                       \
    } while (0)

static int doubles_close(double a, double b) {
    return fabs(a - b) < 1e-12;
}

int main(void) {
    Matrix original = matrix_new(2, 3);
    ASSERT_TRUE(original.data != NULL, "matrix_new should allocate data");

    for (int i = 0; i < original.rows; i++) {
        for (int j = 0; j < original.cols; j++) {
            *matrix_at(&original, i, j) = (double)(i * original.cols + j) + 0.25;
        }
    }

    ASSERT_TRUE(write_matrix_bin("tests/tmp_matrix.bin", &original) == 0,
                "write_matrix_bin should write the matrix");

    Matrix loaded = read_matrix_bin("tests/tmp_matrix.bin");
    ASSERT_TRUE(loaded.rows == original.rows, "loaded row count should match");
    ASSERT_TRUE(loaded.cols == original.cols, "loaded column count should match");
    ASSERT_TRUE(loaded.data != NULL, "read_matrix_bin should allocate data");

    for (int i = 0; i < original.rows; i++) {
        for (int j = 0; j < original.cols; j++) {
            ASSERT_TRUE(doubles_close(*matrix_at(&loaded, i, j),
                                      *matrix_at(&original, i, j)),
                        "loaded matrix values should match original values");
        }
    }

    matrix_free(&loaded);
    matrix_free(&original);
    remove("tests/tmp_matrix.bin");
    return 0;
}

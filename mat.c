#include "mat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static Matrix matrix_empty(void) {
    Matrix matrix = {0, 0, NULL};
    return matrix;
}

static double gaussian_noise(double mean, double stddev) {
    static int have_spare = 0;
    static double spare = 0.0;

    if (have_spare) {
        have_spare = 0;
        return mean + stddev * spare;
    }

    double u;
    double v;
    double s;
    do {
        u = (double)rand() / RAND_MAX * 2.0 - 1.0;
        v = (double)rand() / RAND_MAX * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    have_spare = 1;
    return mean + stddev * (u * s);
}

Matrix matrix_new(int rows, int cols) {
    Matrix matrix = matrix_empty();
    if (rows <= 0 || cols <= 0) {
        return matrix;
    }

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = calloc((size_t)rows * (size_t)cols, sizeof(double));
    if (matrix.data == NULL) {
        perror("Unable to allocate matrix data");
        matrix.rows = 0;
        matrix.cols = 0;
    }

    return matrix;
}

Matrix matrix_random(int rows, int cols, double mean, double stddev) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    Matrix matrix = matrix_new(rows, cols);
    if (matrix.data == NULL) {
        return matrix;
    }

    for (int i = 0; i < rows * cols; i++) {
        matrix.data[i] = gaussian_noise(mean, stddev);
    }

    return matrix;
}

double *matrix_at(Matrix *matrix, int row, int col) {
    return &matrix->data[row * matrix->cols + col];
}

const double *matrix_at_const(const Matrix *matrix, int row, int col) {
    return &matrix->data[row * matrix->cols + col];
}

void matrix_free(Matrix *matrix) {
    if (matrix == NULL) {
        return;
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
}

Matrix read_matrix_bin(const char *filename) {
    Matrix matrix = matrix_empty();
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open matrix file");
        return matrix;
    }

    int rows = 0;
    int cols = 0;
    if (fread(&rows, sizeof(int), 1, file) != 1 ||
        fread(&cols, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Unable to read matrix dimensions from %s\n", filename);
        fclose(file);
        return matrix;
    }

    matrix = matrix_new(rows, cols);
    if (matrix.data == NULL) {
        fclose(file);
        return matrix;
    }

    size_t value_count = (size_t)rows * (size_t)cols;
    if (fread(matrix.data, sizeof(double), value_count, file) != value_count) {
        fprintf(stderr, "Unable to read matrix values from %s\n", filename);
        matrix_free(&matrix);
    }

    fclose(file);
    return matrix;
}

int write_matrix_bin(const char *filename, const Matrix *matrix) {
    if (matrix == NULL || matrix->data == NULL || matrix->rows <= 0 ||
        matrix->cols <= 0) {
        fprintf(stderr, "Cannot write an empty matrix\n");
        return 1;
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Unable to open matrix file for writing");
        return 1;
    }

    size_t value_count = (size_t)matrix->rows * (size_t)matrix->cols;
    int failed = fwrite(&matrix->rows, sizeof(int), 1, file) != 1 ||
                 fwrite(&matrix->cols, sizeof(int), 1, file) != 1 ||
                 fwrite(matrix->data, sizeof(double), value_count, file) !=
                     value_count;

    fclose(file);
    return failed ? 1 : 0;
}

Matrix matmul(Matrix matrix1, Matrix matrix2) {
    if (matrix1.cols != matrix2.rows) {
        printf("Error: Invalid matrix dimensions\n");
        return matrix_empty();
    }

    Matrix result = matrix_new(matrix1.rows, matrix2.cols);
    if (result.data == NULL) {
        return result;
    }

    for (int i = 0; i < matrix1.rows; i++) {
        for (int j = 0; j < matrix2.cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < matrix1.cols; k++) {
                sum += *matrix_at_const(&matrix1, i, k) *
                       *matrix_at_const(&matrix2, k, j);
            }
            *matrix_at(&result, i, j) = sum;
        }
    }

    return result;
}

void matrix_print(const Matrix *matrix) {
    matrix_print_head(matrix, matrix->rows);
}

void matrix_print_head(const Matrix *matrix, int max_rows) {
    int rows = matrix->rows < max_rows ? matrix->rows : max_rows;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%.4f", *matrix_at_const(matrix, i, j));
            if (j < matrix->cols - 1) {
                printf("\t");
            }
        }
        printf("\n");
    }
}
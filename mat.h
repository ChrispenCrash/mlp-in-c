#ifndef MAT_H
#define MAT_H

typedef struct {
    int rows;
    int cols;
    double *data;
} Matrix;

Matrix matrix_new(int rows, int cols);
Matrix matrix_random(int rows, int cols, double mean, double stddev);
Matrix read_matrix_bin(const char *filename);
Matrix matmul(Matrix matrix1, Matrix matrix2);

int write_matrix_bin(const char *filename, const Matrix *matrix);
double *matrix_at(Matrix *matrix, int row, int col);
const double *matrix_at_const(const Matrix *matrix, int row, int col);

void matrix_print(const Matrix *matrix);
void matrix_print_head(const Matrix *matrix, int max_rows);
void matrix_free(Matrix *matrix);

#endif

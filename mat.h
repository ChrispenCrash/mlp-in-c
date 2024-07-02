#ifndef MAT_H
#define MAT_H

typedef struct {
    double **data;
    char **columns;
    int nrows;
    int ncols;
} Matrix;

Matrix read_data(const char *filename);
Matrix matmul(Matrix matrix1, Matrix matrix2);
Matrix new_matrix(int nrows, int ncols);

void print_data(Matrix matrix);
void print_head(Matrix matrix);
void print_tail(Matrix matrix);
void print_cols(Matrix matrix);
void free_data(Matrix matrix);

double generateGaussianNoise(double mean, double stdDev);

#endif

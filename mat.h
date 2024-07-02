#ifndef MAT_H
#define MAT_H

typedef struct {
    double **data;
    char **columns;
    int nrows;
    int ncols;
} Matrix;

Matrix read_data(const char *filename);

void print_data(Matrix dataSet);
void print_head(Matrix dataSet);
void print_tail(Matrix dataSet);
void print_cols(Matrix dataSet);
void free_data(Matrix dataSet);

#endif

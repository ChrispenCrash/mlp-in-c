#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    double **data;
    char **columns;
    int nrows;
    int ncols;
} DataSet;

DataSet read_data(const char *filename);

void print_data(DataSet dataSet);
void print_head(DataSet dataSet);
void print_tail(DataSet dataSet);
void print_cols(DataSet dataSet);
void free_data(DataSet dataSet);

#endif

#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    double **data;
    int rows;
    int cols;
} DataSet;

DataSet read_data(const char *filename);

#endif

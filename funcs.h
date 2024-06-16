#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    double **data;
    char **columns;
    int rows;
    int cols;
} DataSet;

DataSet read_data(const char *filename);

void print_data(DataSet dataSet);
void free_data(DataSet dataSet);

#endif

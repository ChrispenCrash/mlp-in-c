#include "funcs.h"

DataSet matmul(DataSet m1, DataSet m2) {
    if (m1.ncols != m2.nrows) {
        printf("Error: Invalid matrix dimensions\n");
        exit(1);
    }

    DataSet m3;
    m3.nrows = m1.nrows;
    m3.ncols = m2.ncols;

    m3.data = (double **)malloc(m3.nrows * sizeof(double *));

    for (int i = 0; i < m3.nrows; i++) {
        m3.data[i] = (double *)malloc(m3.ncols * sizeof(double));
    }

    for (int i = 0; i < m3.nrows; i++) {
        for (int j = 0; j < m3.ncols; j++) {
            m3.data[i][j] = 0;
            for (int k = 0; k < m1.ncols; k++) {
                m3.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }

    return m3;
}
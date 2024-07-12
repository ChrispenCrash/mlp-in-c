#include <stdio.h>
#include <stdlib.h>

#include "mat.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments\n");
        printf("Correct Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    Matrix dataSet = read_data(filename);

    // print_data(dataSet);

    printf("Dataset has %d rows and %d columns\n", dataSet.nrows,
           dataSet.ncols);

    print_head(dataSet);
    // print_tail(dataSet);
    printf("--------------------\n");

    double noise = generateGaussianNoise(0.0, 0.1);

    printf("Noise: %.02f\n", noise);

    Matrix m_new = new_matrix(10, 10);

    if (m_new.data == NULL) {
        printf("Failed to create new matrix\n");
        free_data(dataSet);
        return 1;
    }

    // print_head(m_new);
    printf("Random number between 0 and 1: %.02f\n", random(0, 1));

    free_data(m_new);
    free_data(dataSet);
    return 0;
}

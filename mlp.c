#include <stdio.h>
#include <stdlib.h>

#include "mat.h"

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        printf("Error: Invalid number of arguments\n");
        printf("Correct Usage: %s <features.bin> [targets.bin]\n", argv[0]);
        return 1;
    }

    Matrix features = read_matrix_bin(argv[1]);
    if (features.data == NULL) {
        return 1;
    }

    printf("Features: %d rows x %d columns\n", features.rows, features.cols);
    matrix_print_head(&features, 5);

    Matrix targets = {0, 0, NULL};
    if (argc == 3) {
        targets = read_matrix_bin(argv[2]);
        if (targets.data == NULL) {
            matrix_free(&features);
            return 1;
        }

        printf("Targets: %d rows x %d columns\n", targets.rows, targets.cols);
        matrix_print_head(&targets, 5);
    }

    printf("--------------------\n");

    Matrix weights = matrix_random(features.cols, 3, 0.0, 0.1);
    if (weights.data == NULL) {
        matrix_free(&targets);
        matrix_free(&features);
        return 1;
    }

    printf("Example random weights: %d rows x %d columns\n", weights.rows,
           weights.cols);
    matrix_print(&weights);

    matrix_free(&weights);
    matrix_free(&targets);
    matrix_free(&features);
    return 0;
}

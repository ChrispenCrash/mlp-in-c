#include <stdio.h>

#include "funcs.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments\n");
        printf("Correct Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    double **data = NULL;
    int rows = 0;
    int cols = 0;

    // Read data from the CSV file
    data = read_data(filename, &rows, &cols);

    // Print the stored data (for verification)
    for (int i = 0; i < rows; i++) {
        printf("%.2f, %.8f\n", data[i][0], data[i][1]);
    }

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}

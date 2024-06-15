#include <stdio.h>
#include <stdlib.h>

#include "funcs.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments\n");
        printf("Correct Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    DataSet dataSet = read_data(filename);

    int head = dataSet.rows > 5 ? 5 : dataSet.rows;
    for (int i = 0; i < head; i++) {
        printf("%.2f, %.8f\n", dataSet.data[i][0], dataSet.data[i][1]);
    }

    // Free allocated memory
    for (int i = 0; i < dataSet.rows; i++) {
        free(dataSet.data[i]);
    }
    free(dataSet.data);

    return 0;
}

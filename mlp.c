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

    print_data(dataSet);

    printf("Dataset has %d rows and %d columns\n", dataSet.nrows,
           dataSet.ncols);

    print_head(dataSet);
    print_tail(dataSet);

    free_data(dataSet);
    return 0;
}

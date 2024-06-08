#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments\n");
        printf("Correct Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Read in csv data
    FILE *fptr;
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    return 0;
}
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void generate_sin_data(char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "x,y\n");

    for (double i = 0.0; i <= 10.0; i += 0.05) {
        fprintf(fptr, "%.1f,%.8f\n", i, sin(i));
    }
    fclose(fptr);
}

int main() {
    generate_sin_data("data/sin_data.csv");
    return 0;
}
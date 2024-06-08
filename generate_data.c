#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double generateGaussianNoise(double mean, double stdDev) {
    static int hasSpare = 0;
    static double spare;

    if (hasSpare) {
        hasSpare = 0;
        return mean + stdDev * spare;
    }

    hasSpare = 1;
    double u, v, s;
    do {
        u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    return mean + stdDev * u * s;
}

void generate_sin_data(char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "x,y\n");
    double mean = 0.0;
    double stdDev = 0.1;

    for (double i = 0.0; i <= 10.0; i += 0.05) {
        double sineValue = sin(i);
        double noisySineValue = sineValue + generateGaussianNoise(mean, stdDev);
        fprintf(fptr, "%.1f,%.8f\n", i, noisySineValue);
    }
    fclose(fptr);
}

void generate_xnor_data(char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "x1,x2,y\n");

    for (int i = 0; i < 100; i++) {
        int x1 = rand() % 2;
        int x2 = rand() % 2;
        int y = (x1 == x2) ? 1 : 0;
        fprintf(fptr, "%d,%d,%d\n", x1, x2, y);
    }
}

int main() {
    generate_sin_data("data/sin_data.csv");
    generate_xnor_data("data/xnor_data.csv");
    return 0;
}
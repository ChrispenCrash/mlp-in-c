#include "mat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_CAPACITY 10

Matrix read_data(const char *filename) {
    Matrix m;
    m.data = NULL;
    m.columns = NULL;
    m.nrows = 0;
    m.ncols = 0;

    FILE *file;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    char *token;
    int capacity = INITIAL_CAPACITY;

    // Open the CSV file
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return m;
    }

    // Read the first line (header) to determine the number of columns and store
    // column names
    if ((read = getline(&line, &len, file)) != -1) {
        // Count the number of columns in the header
        token = strtok(line, ",");
        while (token != NULL) {
            m.ncols++;
            token = strtok(NULL, ",");
        }

        // Allocate memory for columns
        m.columns = malloc(m.ncols * sizeof(char *));
        if (m.columns == NULL) {
            perror("Unable to allocate memory for columns");
            fclose(file);
            free(line);
            return m;
        }

        // Store column names
        rewind(file);  // Reset file pointer to beginning of file
        read = getline(&line, &len, file);
        token = strtok(line, ",");
        int col_idx = 0;
        while (token != NULL) {
            m.columns[col_idx] = strdup(token);
            if (m.columns[col_idx] == NULL) {
                perror("Unable to allocate memory for column name");
                fclose(file);
                free(line);
                return m;
            }
            col_idx++;
            token = strtok(NULL, ",");
        }
    }

    // Allocate initial memory for data array (rows)
    m.data = malloc(capacity * sizeof(double *));
    if (m.data == NULL) {
        perror("Unable to allocate memory");
        fclose(file);
        free(line);
        return m;
    }

    // Read each line from the file
    while ((read = getline(&line, &len, file)) != -1) {
        // Reallocate memory if row capacity is reached
        if (m.nrows >= capacity) {
            capacity *= 2;
            m.data = realloc(m.data, capacity * sizeof(double *));
            if (m.data == NULL) {
                perror("Unable to reallocate memory");
                fclose(file);
                free(line);
                return m;
            }
        }

        // Allocate memory for each row (columns)
        m.data[m.nrows] = malloc(m.ncols * sizeof(double));
        if (m.data[m.nrows] == NULL) {
            perror("Unable to allocate memory for row");
            fclose(file);
            free(line);
            return m;
        }

        // Split the line into tokens (columns) and store as double
        int col = 0;
        token = strtok(line, ",");
        while (token != NULL) {
            m.data[m.nrows][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }

        m.nrows++;
    }

    // Close the file and free the line buffer
    fclose(file);
    free(line);

    return m;
}

void print_cols(Matrix m) {
    for (int j = 0; j < m.ncols; j++) {
        if (j < m.ncols - 1) {
            printf("%s\t", m.columns[j]);
        } else {
            printf("%s", m.columns[j]);
        }
    }
}

void print_data(Matrix m) {
    for (int i = 0; i < m.nrows; i++) {
        for (int j = 0; j < m.ncols; j++) {
            printf("%.02f\t", m.data[i][j]);
        }
        printf("\n");
    }
}

void print_head(Matrix m) {
    print_cols(m);

    int head = m.nrows > 5 ? 5 : m.nrows;
    for (int i = 0; i < head; i++) {
        for (int j = 0; j < m.ncols; j++) {
            printf("%.02f\t", m.data[i][j]);
        }
        printf("\n");
    }
}

void print_tail(Matrix m) {
    print_cols(m);

    int tail = m.nrows > 5 ? m.nrows - 5 : 0;

    for (int i = tail; i < m.nrows; i++) {
        for (int j = 0; j < m.ncols; j++) {
            printf("%.02f\t", m.data[i][j]);
        }
        printf("\n");
    }
}

void free_data(Matrix m) {
    for (int i = 0; i < m.nrows; i++) {
        free(m.data[i]);
    }
    free(m.data);

    for (int j = 0; j < m.ncols; j++) {
        free(m.columns[j]);
    }
    free(m.columns);
}

Matrix matmul(Matrix matrix1, Matrix matrix2) {
    Matrix m3;
    m3.data = NULL;
    m3.columns = NULL;
    m3.nrows = 0;
    m3.ncols = 0;

    if (matrix1.ncols != matrix2.nrows) {
        printf("Error: Invalid matrix dimensions\n");
        return m3;
    }

    m3.nrows = matrix1.nrows;
    m3.ncols = matrix2.ncols;

    m3.data = malloc(m3.nrows * sizeof(double *));
    if (m3.data == NULL) {
        perror("Unable to allocate memory");
        return m3;
    }

    for (int i = 0; i < m3.nrows; i++) {
        m3.data[i] = malloc(m3.ncols * sizeof(double));
        if (m3.data[i] == NULL) {
            perror("Unable to allocate memory");
            return m3;
        }
    }

    for (int i = 0; i < matrix1.nrows; i++) {
        for (int j = 0; j < matrix2.ncols; j++) {
            m3.data[i][j] = 0;
            for (int k = 0; k < matrix1.ncols; k++) {
                m3.data[i][j] += matrix1.data[i][k] * matrix2.data[k][j];
            }
        }
    }

    return m3;
}

Matrix new_matrix(int nrows, int ncols) {
    Matrix m;
    m.data = NULL;
    m.columns = NULL;
    m.nrows = nrows;
    m.ncols = ncols;

    m.data = malloc(nrows * sizeof(double *));
    if (m.data == NULL) {
        perror("Unable to allocate memory for rows");
        return m;
    }

    for (int i = 0; i < nrows; i++) {
        m.data[i] = malloc(ncols * sizeof(double));
        if (m.data[i] == NULL) {
            perror("Unable to allocate memory for columns");
            for (int j = 0; j < i; j++) {
                free(m.data[j]);
            }
            free(m.data);
            m.data = NULL;  // Reset data pointer to indicate failure
            return m;
        }
    }

    // Fill matrix with random Gaussian noise
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            m.data[i][j] = generateGaussianNoise(0.0, 0.1);
            // m.data[i][j] = random(-1.0, 1.0);
        }
    }

    return m;
}

double generateGaussianNoise(double mean, double stddev) {
    srand(time(NULL));
    static int haveSpare = 0;
    static double spare;

    if (haveSpare) {
        haveSpare = 0;
        return mean + stddev * spare;
    }

    haveSpare = 1;
    double u, v, s;
    do {
        u = (double)rand() / RAND_MAX * 2.0 - 1.0;
        v = (double)rand() / RAND_MAX * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    return mean + stddev * (u * s);
}

double random(double min, double max) {
    srand(time(NULL));
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
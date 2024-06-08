#include "funcs.h"

#include <string.h>

double **read_data(const char *filename, int *rows, int *cols) {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    int read;
    char *token;
    int row_count = 0;
    int col_count = 0;
    int capacity = INITIAL_CAPACITY;
    double **data = NULL;

    // Open the CSV file
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Read the first line (header) to determine the number of columns
    if ((read = getline(&line, &len, file)) != -1) {
        // Count the number of columns in the header
        token = strtok(line, ",");
        while (token != NULL) {
            col_count++;
            token = strtok(NULL, ",");
        }
    }

    // Allocate initial memory for data array (rows)
    data = malloc(capacity * sizeof(double *));
    if (data == NULL) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Read each line from the file
    while ((read = getline(&line, &len, file)) != -1) {
        if (row_count == 0) {
            // Skip the header row
            row_count++;
            continue;
        }

        // Reallocate memory if row capacity is reached
        if (row_count >= capacity) {
            capacity *= 2;
            data = realloc(data, capacity * sizeof(double *));
            if (data == NULL) {
                perror("Unable to reallocate memory");
                exit(EXIT_FAILURE);
            }
        }

        // Allocate memory for each row (columns)
        data[row_count] = malloc(col_count * sizeof(double));
        if (data[row_count] == NULL) {
            perror("Unable to allocate memory for row");
            exit(EXIT_FAILURE);
        }

        // Split the line into tokens (columns) and store as double
        int col = 0;
        token = strtok(line, ",");
        while (token != NULL) {
            data[row_count][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }

        row_count++;
    }

    // Close the file and free the line buffer
    fclose(file);
    free(line);

    // Set the output parameters
    *rows = row_count;
    *cols = col_count;

    return data;
}

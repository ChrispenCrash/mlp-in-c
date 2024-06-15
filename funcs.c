#include "funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

DataSet read_data(const char *filename) {
    DataSet result;
    result.data = NULL;
    result.rows = 0;
    result.cols = 0;
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
        return result;
    }

    // Read the first line (header) to determine the number of columns
    if ((read = getline(&line, &len, file)) != -1) {
        // Count the number of columns in the header
        token = strtok(line, ",");
        while (token != NULL) {
            result.cols++;
            token = strtok(NULL, ",");
        }
    }

    // Allocate initial memory for data array (rows)
    result.data = malloc(capacity * sizeof(double *));
    if (result.data == NULL) {
        perror("Unable to allocate memory");
        fclose(file);
        free(line);
        return result;
    }

    // Read each line from the file
    while ((read = getline(&line, &len, file)) != -1) {
        if (result.rows == 0) {
            // Skip the header row
            result.rows++;
            continue;
        }

        // Reallocate memory if row capacity is reached
        if (result.rows >= capacity) {
            capacity *= 2;
            result.data = realloc(result.data, capacity * sizeof(double *));
            if (result.data == NULL) {
                perror("Unable to reallocate memory");
                fclose(file);
                free(line);
                return result;
            }
        }

        // Allocate memory for each row (columns)
        result.data[result.rows] = malloc(result.cols * sizeof(double));
        if (result.data[result.rows] == NULL) {
            perror("Unable to allocate memory for row");
            fclose(file);
            free(line);
            return result;
        }

        // Split the line into tokens (columns) and store as double
        int col = 0;
        token = strtok(line, ",");
        while (token != NULL) {
            result.data[result.rows][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }

        result.rows++;
    }

    // Close the file and free the line buffer
    fclose(file);
    free(line);

    return result;
}

void print_data(DataSet dataSet) {
    int head = dataSet.rows > 5 ? 5 : dataSet.rows;
    for (int i = 0; i < head; i++) {
        for (int j = 0; j < dataSet.cols; j++) {
            printf("%.02f", dataSet.data[i][j]);
            if (j < dataSet.cols - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void free_data(DataSet dataSet) {
    for (int i = 0; i < dataSet.rows; i++) {
        free(dataSet.data[i]);
    }
    free(dataSet.data);
}
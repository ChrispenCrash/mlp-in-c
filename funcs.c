#include "funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

DataSet read_data(const char *filename) {
    DataSet result;
    result.data = NULL;
    result.columns = NULL;
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

    // Read the first line (header) to determine the number of columns and store
    // column names
    if ((read = getline(&line, &len, file)) != -1) {
        // Count the number of columns in the header
        token = strtok(line, ",");
        while (token != NULL) {
            result.cols++;
            token = strtok(NULL, ",");
        }

        // Allocate memory for columns
        result.columns = malloc(result.cols * sizeof(char *));
        if (result.columns == NULL) {
            perror("Unable to allocate memory for columns");
            fclose(file);
            free(line);
            return result;
        }

        // Store column names
        rewind(file);  // Reset file pointer to beginning of file
        read = getline(&line, &len, file);
        token = strtok(line, ",");
        int col_idx = 0;
        while (token != NULL) {
            result.columns[col_idx] = strdup(token);
            if (result.columns[col_idx] == NULL) {
                perror("Unable to allocate memory for column name");
                fclose(file);
                free(line);
                return result;
            }
            col_idx++;
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

    // Read each line from the file (skipping the header)
    int is_header = 1;  // flag to check header row
    while ((read = getline(&line, &len, file)) != -1) {
        if (is_header) {
            is_header = 0;
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
    // Print column names
    for (int j = 0; j < dataSet.cols; j++) {
        printf("%s", dataSet.columns[j]);
        if (j < dataSet.cols - 1) {
            printf(", ");
        }
    }
    printf("\n");

    // Print data
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

    for (int j = 0; j < dataSet.cols; j++) {
        free(dataSet.columns[j]);
    }
    free(dataSet.columns);
}

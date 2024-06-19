#include "funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

DataSet read_data(const char *filename) {
    DataSet result;
    result.data = NULL;
    result.columns = NULL;
    result.nrows = 0;
    result.ncols = 0;

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
            result.ncols++;
            token = strtok(NULL, ",");
        }

        // Allocate memory for columns
        result.columns = malloc(result.ncols * sizeof(char *));
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
    int is_header = 0;  // flag to check header row
    while ((read = getline(&line, &len, file)) != -1) {
        if (is_header) {
            is_header = 0;
            continue;
        }

        // Reallocate memory if row capacity is reached
        if (result.nrows >= capacity) {
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
        result.data[result.nrows] = malloc(result.ncols * sizeof(double));
        if (result.data[result.nrows] == NULL) {
            perror("Unable to allocate memory for row");
            fclose(file);
            free(line);
            return result;
        }

        // Split the line into tokens (columns) and store as double
        int col = 0;
        token = strtok(line, ",");
        while (token != NULL) {
            result.data[result.nrows][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }

        result.nrows++;
    }

    // Close the file and free the line buffer
    fclose(file);
    free(line);

    return result;
}

void print_cols(DataSet dataSet) {
    for (int j = 0; j < dataSet.ncols; j++) {
        // printf("%s\t", dataSet.columns[j]);
        // Print columns with tab unless it's the last column
        if (j < dataSet.ncols - 1) {
            printf("%s\t", dataSet.columns[j]);
        } else {
            printf("%s", dataSet.columns[j]);
        }
    }
}

void print_data(DataSet dataSet) {
    print_cols(dataSet);

    for (int i = 0; i < dataSet.nrows; i++) {
        for (int j = 0; j < dataSet.ncols; j++) {
            printf("%.02f", dataSet.data[i][j]);
            if (j < dataSet.ncols - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void print_head(DataSet dataSet) {
    print_cols(dataSet);

    int head = dataSet.nrows > 5 ? 5 : dataSet.nrows;
    for (int i = 0; i < head; i++) {
        for (int j = 0; j < dataSet.ncols; j++) {
            printf("%.02f", dataSet.data[i][j]);
            if (j < dataSet.ncols - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void print_tail(DataSet dataSet) {
    print_cols(dataSet);

    int tail = dataSet.nrows > 5 ? dataSet.nrows - 5 : 0;

    for (int i = tail; i < dataSet.nrows; i++) {
        for (int j = 0; j < dataSet.ncols; j++) {
            printf("%.02f", dataSet.data[i][j]);
            if (j < dataSet.ncols - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void free_data(DataSet dataSet) {
    for (int i = 0; i < dataSet.nrows; i++) {
        free(dataSet.data[i]);
    }
    free(dataSet.data);

    for (int j = 0; j < dataSet.ncols; j++) {
        free(dataSet.columns[j]);
    }
    free(dataSet.columns);
}

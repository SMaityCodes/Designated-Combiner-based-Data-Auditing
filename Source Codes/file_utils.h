#include <pbc/pbc.h>
#include <pbc/pbc_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Opens a file with the specified mode and exits if the file cannot be opened.
FILE* open_file(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

// Save PBC_element to file
void save_element_to_file(element_t X, FILE *fptr) {
    size_t size = element_length_in_bytes(X);
    unsigned char *bin = (unsigned char *) malloc(size);
    if (!bin) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    element_to_bytes(bin, X);
    if (fwrite(bin, 1, size, fptr) != size) {
        perror("Error saving element to file");
        free(bin);
        exit(EXIT_FAILURE);
    }
    
    free(bin);
}

// Read PBC_element from file
void read_element_from_file(element_t X, FILE *fptr) {
    size_t size = element_length_in_bytes(X);
    unsigned char *bin = (unsigned char *) malloc(size);
    if (!bin) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    if (fread(bin, 1, size, fptr) != size) {
        perror("Error reading from file");
        free(bin);
        exit(EXIT_FAILURE);
    }
    
    element_from_bytes(X, bin);
    free(bin);
}

// Read integer from file and handle file end
void read_integer_from_file(FILE* file, char* integer_str, size_t size) {
    if (!fgets(integer_str, size, file)) {
        rewind(file);
        if (!fgets(integer_str, size, file)) {
            perror("Error reading integer from file");
            exit(EXIT_FAILURE);
        }
    }
    
    size_t len = strlen(integer_str);
    if (len > 0 && integer_str[len-1] == '\n') {
        integer_str[len-1] = '\0';
    }
}

// Write sorted numbers to the file
void write_numbers_to_file(char *filename, int *numbers, int num_blocks) {
    FILE *file = open_file(filename, "w");
    
    for (int i = 0; i < num_blocks; i++) {
        fprintf(file, "%d\n", numbers[i]);
        if (ferror(file)) {
            perror("Error writing to file");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

// Function to read the next integer from a file and return its value
int read_next_integer(FILE *file) {
    int value;
    int result = fscanf(file, "%d", &value);
    if (result == 1) {
        return value;  // Successfully read an integer
    } else {
        if (feof(file)) {
            //printf("End of file reached.\n");
        } else {
            printf("Error reading integer from file.\n");
        }
        return -1;  // Return -1 if an error occurs
    }
}

// User-defined function to truncate at '@' and concatenate a given string
void truncate_and_concat(char *ID, char *str) {
    // Find the '@' character in ID
    char *at_ptr = strchr(ID, '@');
    if (at_ptr != NULL) {
        // Replace '@' with null terminator to truncate the string
        *at_ptr = '\0';
    }
    // Concatenate the given string 'str' to the truncated ID
    strcat(ID, str);
}

FILE *create_and_open_file(char *id, char *suffix, char *mode) {
    char filename[50];
    
    strcpy(filename, id);
    truncate_and_concat(filename, suffix);
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error opening file");
    }
    return file;
}

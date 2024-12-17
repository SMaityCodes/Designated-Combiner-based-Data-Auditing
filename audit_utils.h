#include "pbc_utils.h"

#define BLOCK_SIZE 1000
#define SEED_SIZE 32

// Generate integers from 1 to iterations in a file
void genint(char* filename, long long iterations) {
    FILE *file = open_file(filename, "w");
    
    for (long long i = 1; i <= iterations; i++) {
        fprintf(file, "%lld\n", i);
    }
    
    fclose(file);
}

// Function to read the seed from the binary file
void read_seed(char *seed_file_name, char *seed) {
    FILE *seedFile = open_file(seed_file_name, "rb");
    if (fread(seed, 1, SEED_SIZE, seedFile) != SEED_SIZE) {
        printf("Error: Could not read seed from %s\n", seed_file_name);
        fclose(seedFile);
        exit(EXIT_FAILURE);
    }
    fclose(seedFile);
}

// Process and hash function(H2)
void process_and_hash2(long long iterations, char* filename, char* id_f, char* filename1, element_t Pe) {
    element_t result;
    element_init_G1(result, global_params);
    char file1[50];
    size_t offset = 0;
    strcpy(file1 + offset, id_f);
    offset += strlen(id_f);

    char integer_str[20];
    FILE *file = open_file(filename, "r");
    
    FILE *H2_write = open_file(filename1, "wb");
    
    for (long long i = 1; i <= iterations; i++) {
        read_integer_from_file(file, integer_str, sizeof(integer_str));
        strcpy(file1 + offset, integer_str);
        element_from_hash(result, file1, strlen(file1));
        save_element_to_file(result, H2_write);
    }
    fclose(file);
    fclose(H2_write);
}

// Generates a deterministic PBC_element from Zr using a seed and index for unique randomness.
void generate_deterministic_v_with_seed(element_t v, const char* seed_str, int index) {
    unsigned long seed = strtoul(seed_str, NULL, 10) + index;  // Vary seed by index for distinct points

    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui(rand_state, seed);

    mpz_t rand_int;
    mpz_init(rand_int);
    mpz_urandomb(rand_int, rand_state, 256); // Generate a random number of 256 bits

    element_init_Zr(v, global_params);

    char rand_str[65];
    mpz_get_str(rand_str, 16, rand_int);  // Convert random integer to string (hex format)
    rand_str[64] = '\0';  // Ensure null-termination

    element_from_hash(v, rand_str, strlen(rand_str));

    mpz_clear(rand_int);
    gmp_randclear(rand_state);
}

// Function to check if a number is already in the array
int is_in_array(int num, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == num) {
            return 1; // Number is already in the array
        }
    }
    return 0; // Number is not in the array
}

// Generate distinct random numbers
void generate_unique_random_numbers(int lower, int upper, int num_blocks, int *numbers) {
    int num_generated = 0;
    while (num_generated < num_blocks) {
        int num = lower + rand() % (upper - lower + 1);
        if (!is_in_array(num, numbers, num_generated)) {
            numbers[num_generated] = num;
            num_generated++;
        }
    }
}

// Comparison function for qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to initialize random seed
unsigned int initialize_random_seed(const unsigned char *seed) {
    unsigned int seedI;
    memcpy(&seedI, seed, sizeof(seedI)); // Copy first 4 bytes of seed into seedI
    srand(seedI);  // Initialize the random number generator
    return seedI;
}

// Main function or calling function to demonstrate usage
void process_numbers(const unsigned char *seed, int num_blocks, int block_count, char *filename) {

    initialize_random_seed(seed);

    // Define range and allocate memory for numbers array
    int lower = 1, upper = block_count;
    int *numbers = malloc(num_blocks * sizeof(int));
    if (!numbers) {
        perror("Failed to allocate memory for numbers array");
        exit(EXIT_FAILURE);
    }

    // Generate unique random numbers
    generate_unique_random_numbers(lower, upper, num_blocks, numbers);

    // Sort the generated numbers
    qsort(numbers, num_blocks, sizeof(int), compare);

    // Write sorted numbers to the file
    write_numbers_to_file(filename, numbers, num_blocks);

    // Cleanup
    free(numbers);
}

int read_challenge_file(FILE *chalFile, unsigned char *seed, float *num) {
    if (fread(seed, 1, SEED_SIZE, chalFile) != SEED_SIZE) {
        printf("Error: Could not read the seed from the challenge file.\n");
        exit(EXIT_FAILURE);
    }
    
    if (fscanf(chalFile, "%f", num) != 1) {
        printf("Error: Could not read the challenge percentage from the challenge file.\n");
        exit(EXIT_FAILURE);
    }
    
    return 1;
}

void write_to_file(char *filename, char *id_f, long long num_blocks) {
    FILE *file = open_file(filename, "w");
    fprintf(file, "%s\n", id_f);
    fprintf(file, "%lld\n", num_blocks);
    fclose(file);
}

void read_from_file(char *filename, char **fileName, long long *num_blocks) {
    FILE *file = open_file(filename, "r");
    size_t len = 0;
    ssize_t read;

    getline(fileName, &len, file);
    (*fileName)[strcspn(*fileName, "\n")] = '\0';

    fscanf(file, "%lld", num_blocks);
    fclose(file);
}

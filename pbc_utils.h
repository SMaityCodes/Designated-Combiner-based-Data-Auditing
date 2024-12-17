#include <sys/stat.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "file_utils.h"

bool debug = 0;
bool lastDebug = 1;

// Global pairing parameters
pairing_t global_params;

// Data structures for setup and key generation
typedef struct {
    element_t g;
    element_t g0;
} IBEPARAMS;
typedef struct {
    element_t alpha;
    IBEPARAMS params;
} SETUPVALS;
typedef struct {
    element_t beta;
    element_t P;
} PUKEYVALS;

// Hash function for ID
void H1(element_t Q, char *str) {
    element_from_hash(Q, str, strlen(str));
}

float measure_time(clock_t start, clock_t end) {
    return (float)(end - start) / CLOCKS_PER_SEC * 1000;
}

// Setup function
SETUPVALS setup(float *totalTimeTaken) {
    if(debug) {
    printf("SETUP ALGO INVOKED...\n");
    }
    
    SETUPVALS setup_vals;
    element_init_G1(setup_vals.params.g, global_params);
    element_init_G1(setup_vals.params.g0, global_params);
    element_init_Zr(setup_vals.alpha, global_params);
    
    clock_t startTime, endTime;
    
    startTime = clock();
    element_random(setup_vals.params.g);
    element_random(setup_vals.alpha);
    element_pow_zn(setup_vals.params.g0, setup_vals.params.g, setup_vals.alpha);
    endTime = clock();
    
    *totalTimeTaken = measure_time(startTime, endTime);
    
    return setup_vals;
}

// Key generation function 1
void keygen1(element_t D, element_t alpha, char *ID, float *totalTimeTaken) {
    if(debug) {
    printf("KEYGEN-1 ALGO INVOKED for %s...\n", ID);
    }
    
    element_t Q;  
    element_init_G1(Q, global_params);
    
    clock_t startTime, endTime;
    
    startTime = clock();
    H1(Q, ID);
    element_pow_zn(D, Q, alpha);
    endTime = clock();
    
    *totalTimeTaken = measure_time(startTime, endTime);
    
    element_clear(Q);
}

// Key generation function 2
PUKEYVALS keygen2(element_t g, char *ID, float *totalTimeTaken) {
    if(debug) {
    printf("KEYGEN-2 ALGO INVOKED for %s...\n", ID);
    }
    
    PUKEYVALS keyvals;
    element_init_G1(keyvals.P, global_params);
    element_init_Zr(keyvals.beta, global_params);
    
    clock_t startTime, endTime;
    
    startTime = clock();
    element_random(keyvals.beta);
    element_pow_zn(keyvals.P, g, keyvals.beta);
    endTime = clock();
    
    *totalTimeTaken = measure_time(startTime, endTime);
    
    return keyvals;
}

// Initialize PBC library with parameters from file
void myPBC_Initialize(char *arg1) {   
    FILE *param_file = open_file(arg1, "r");
    
    // Initialize PBC parameters using command line
    char *arr[2] = {" ", arg1};
    pbc_demo_pairing_init(global_params, 2, arr);
    
    fclose(param_file);
}

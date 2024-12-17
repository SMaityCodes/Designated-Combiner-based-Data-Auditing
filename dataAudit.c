#include "audit_utils.h"

void handle_setup(FILE *msk_file, FILE *params_file, SETUPVALS setup_vals) {
    save_element_to_file(setup_vals.alpha, msk_file);
    save_element_to_file(setup_vals.params.g, params_file);
    save_element_to_file(setup_vals.params.g0, params_file);
    fclose(msk_file);
    fclose(params_file);
    element_clear(setup_vals.alpha);
    element_clear(setup_vals.params.g);
    element_clear(setup_vals.params.g0);
}

void setup_main() {    
    SETUPVALS setup_vals;
    
    FILE *msk_file = open_file("MSK.bin", "wb");
    FILE *params_file = open_file("localParams.bin", "wb");
    
    FILE *stat_file = open_file("statistics.txt", "a");
    float totalTimeTaken = 0.0;
    
    setup_vals = setup(&totalTimeTaken);
        
    handle_setup(msk_file, params_file, setup_vals);
    
    fprintf(stat_file, "Setup Phase Time = %.2f ms\n", totalTimeTaken);
    fclose(stat_file);
    
    if(debug) {
    printf("Setup Executed Successfully. \nMSK.bin and localParams.bin are generated\n\n");
    }
}

void handle_keygen1(FILE *privt_key_file, element_t private_key) {
    save_element_to_file(private_key, privt_key_file);
    fclose(privt_key_file);
    element_clear(private_key);
}

void partialKeyGen_main(int argc, char **argv) {    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <MSK file> <ID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    element_t alpha, private_key;
    element_init_Zr(alpha, global_params);
    element_init_G1(private_key, global_params);
        
    FILE *msk_file = open_file(argv[1], "rb");
    
    char *ID = argv[2];
    
    FILE *privt_key_file = create_and_open_file(ID, "_partial_private_key.bin", "wb");
    
    read_element_from_file(alpha, msk_file);
    
    FILE *stat_file = open_file("statistics.txt", "a");
    float totalTimeTaken = 0.0;
    
    keygen1(private_key, alpha, ID, &totalTimeTaken);
       
    handle_keygen1(privt_key_file, private_key);
    
    fclose(msk_file);
    element_clear(alpha);
    
    fprintf(stat_file, "Partial Key Generation Phase Time = %.2f ms\n", totalTimeTaken);
    fclose(stat_file);
    
    if(debug) {
    printf("Partial Key Generation Executed Successfully.");
    }
}

void fullKeyGen_main(int argc, char **argv) {    
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <local params file> <partial private key file> <ID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    element_t g, g0, par_private_key, Q, P1, P2;
    PUKEYVALS key_vals;
    element_init_G1(g, global_params);
    element_init_G1(g0, global_params);
    element_init_G1(par_private_key, global_params);
    element_init_G1(Q, global_params);
    element_init_GT(P1, global_params);
    element_init_GT(P2, global_params);
    
    FILE *params_file = open_file(argv[1], "rb");
    FILE *par_privt_key_file = open_file(argv[2], "rb");
    char *ID = argv[3];
    
    FILE *pub_key_file = create_and_open_file(ID, "_public_key.bin", "wb");
    FILE *full_privt_key_file = create_and_open_file(ID, "_full_private_key.bin", "wb");
    
    read_element_from_file(g, params_file);
    read_element_from_file(g0, params_file);
    read_element_from_file(par_private_key, par_privt_key_file);
    
    FILE *stat_file = open_file("statistics.txt", "a");
    clock_t startTime, endTime;
    float totalTimeTaken = 0.0;
    
    startTime = clock();
    H1(Q, ID);
    element_pairing(P1, par_private_key, g);
    element_pairing(P2, Q, g0);
    
    if (!element_cmp(P1, P2)) {
        key_vals = keygen2(g, ID, &totalTimeTaken);
    }
    else {
        printf("\nAuthentication failed in full key generation phase\n");
    }
    endTime = clock();
    
    save_element_to_file(key_vals.beta, full_privt_key_file);
    save_element_to_file(par_private_key, full_privt_key_file);
    save_element_to_file(key_vals.P, pub_key_file);
    
    fclose(params_file);
    fclose(par_privt_key_file);
    fclose(full_privt_key_file);
    fclose(pub_key_file);
    element_clear(g);
    element_clear(g0);
    element_clear(par_private_key);
    element_clear(Q);
    element_clear(P1);
    element_clear(P2);
    element_clear(key_vals.beta);
    element_clear(key_vals.P);
    
    fprintf(stat_file, "Full Key Generation Phase Time = %.2f ms\n", totalTimeTaken + measure_time(startTime, endTime));
    fclose(stat_file);
    
    if(debug) {
    printf("Full Key Generation Executed Successfully.");
    }
}

void taggen(char *input_file, char *output_file, element_t Dc, element_t Pe, element_t Bc, float *totalTimeTaken, long long *blocks) {
    if(debug) {
        printf("TAG GEN ALGO INVOKED...\n\n");
    }
    
    FILE *fptr1 = open_file(input_file, "rb");
    FILE *Sigma_write = open_file(output_file, "wb");
       
    long long num_blocks;
    struct stat st;
    stat(input_file, &st);
    
    num_blocks = (st.st_size / BLOCK_SIZE) + 1;  // Calculate number of blocks
    *blocks = num_blocks;
    
    if(debug) {
        printf("Total number of blocks: %lld\n", num_blocks);
    }

    genint("integer.txt", num_blocks);
    process_and_hash2(num_blocks, "integer.txt", input_file, "H2TG.bin", Pe);
    
    FILE *H2_read = open_file("H2TG.bin", "rb");
        
    unsigned char buffer[BLOCK_SIZE];
    size_t bytes_read;
    element_t bl1, j1, j2, j3, j4, j5, result;

    element_init_Zr(bl1, global_params);
    element_init_G1(j1, global_params);
    element_init_G1(j2, global_params);
    element_init_G1(j3, global_params);
    element_init_G1(j4, global_params);
    element_init_G1(j5, global_params);
    element_init_G1(result, global_params);

    char file1[50];
    
    int i = 0;
    clock_t startTime, endTime;
    
    while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, fptr1)) > 0) {
        if(debug) {
            printf("\nProcessing Block %d...\n", i + 1);
        }

        read_element_from_file(j2, H2_read);
        
        startTime = clock();
        element_from_hash(result, file1, strlen(file1));
        element_from_hash(bl1, buffer, bytes_read);
        element_pow_zn(j1, Dc, bl1);
        element_mul(j3, j2, Pe);
        element_pow_zn(j4, j3, Bc);
        element_mul(j5, j1, j4);
        endTime = clock();    

        save_element_to_file(j5, Sigma_write);
        
        *totalTimeTaken += measure_time(startTime, endTime);

        i++;
    }

    fclose(fptr1);
    fclose(H2_read);
    fclose(Sigma_write);
    
    // Clean up elements
    element_clear(result);
    element_clear(bl1);
    element_clear(j1);
    element_clear(j2);
    element_clear(j3);
    element_clear(j4);
    element_clear(j5);
}

void tagGen_main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <csp full private key file> <auditee public key file> <input file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    element_t Dc, Bc, Pe;
    element_init_G1(Dc, global_params);
    element_init_Zr(Bc, global_params);
    element_init_G1(Pe, global_params);

    FILE *privt_key_csp_file = open_file(argv[1], "rb");
    FILE *pub_key_auditee_file = open_file(argv[2], "rb");

    read_element_from_file(Bc, privt_key_csp_file);
    read_element_from_file(Dc, privt_key_csp_file);
    read_element_from_file(Pe, pub_key_auditee_file);
    
    long long num_blocks;
    
    FILE *stat_file = open_file("statistics.txt", "a");
    float totalTimeTaken = 0.0;

    taggen(argv[3], "sigma.bin", Dc, Pe, Bc, &totalTimeTaken, &num_blocks);
    
    write_to_file("file_info.txt", argv[3], num_blocks);
    
    // Clean up
    fclose(privt_key_csp_file);
    fclose(pub_key_auditee_file);
    element_clear(Dc);
    element_clear(Bc);
    element_clear(Pe);

    fprintf(stat_file, "Tag(one block) Generation for  Time(avg) = %.2f ms\n", totalTimeTaken/num_blocks);
    fclose(stat_file);

    if(debug) {
    printf("Tag Generation Executed Successfully. \nSave metadata on file name %s\n\n", "sigma.bin");
    }
}

void seed_pbc_random(FILE *chal_file) {
    FILE *random_file = open_file("/dev/urandom", "r");
    
    // Read some random bytes (for example, 32 bytes) from /dev/urandom
    unsigned char seed[SEED_SIZE];
    if (fread(seed, 1, sizeof(seed), random_file) != sizeof(seed)) {
        printf("Error: Could not read random data from /dev/urandom\n");
        fclose(random_file);
        exit(EXIT_FAILURE);
    }
    fclose(random_file);

    // Write the seed to chal_file
    if (fwrite(seed, 1, sizeof(seed), chal_file) != sizeof(seed)) {
        printf("Error: Could not write seed to chal_file.txt\n");
        exit(EXIT_FAILURE);
    }

    // Add a newline for separation in the output file
    fprintf(chal_file, "\n");
    
    if(debug) {
    printf("\nSeed successfully written to chal_file.txt.\n\n");
    }
}

void chalGen_main(int argc, char **argv) {
    // Open chal_file.txt for writing both the seed and number
    FILE *chal_file = open_file("chal_file.txt", "wb");
    
    // Write random seed to chal_file.txt
    seed_pbc_random(chal_file);
    
    // Checks whether at least 1 command line argument is given
    if(argc < 2){
        printf("Error: Please Enter Correct Execution Command %d !!!\n", argc);
        fclose(chal_file);
        exit(1);
    }
    
    // Convert the argument to a float and write it to chal_file.txt
    float number = atof(argv[1]);
    fprintf(chal_file, "%f\n", number);
     
    fclose(chal_file);
    
    if(debug) {
    printf("Challenge Generation Executed Successfully.");
    }
}

void proofgen(char *arg1, char *arg2, char *arg3, char *arg4, element_t Be, element_t Pc) {
    if(debug) {
        printf("PROOF GEN ALGO INVOKED...\n\n");
    }
	
    unsigned char buffer[BLOCK_SIZE];
    size_t bytes_read;
    int i = 0;
    	
    element_t bl1, Zr_point1, mu, add_mu, add_Zr_points, sigu, pro_sigu, j1, j2, j3, j4, j5, sig;

    element_init_Zr(bl1, global_params);
    element_init_Zr(Zr_point1, global_params);
    element_init_Zr(add_Zr_points, global_params);
    element_init_Zr(mu, global_params);
    element_init_Zr(add_mu, global_params);
    element_init_Zr(j1, global_params);
    element_init_Zr(j2, global_params);

    element_init_G1(sigu, global_params);
    element_init_G1(pro_sigu, global_params);
    element_init_G1(sig, global_params);
    element_init_G1(j3, global_params);
    element_init_G1(j4, global_params);
    element_init_G1(j5, global_params);
    	
    FILE *fptr1 = open_file(arg1, "rb");   	
    FILE *Sigma_read = open_file(arg2, "rb");
    FILE *POP_write = open_file(arg3, "wb");
    FILE *chalFile = open_file(arg4, "rb");
    	
    while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, fptr1)) > 0) {
     	i++;
    }
    fclose(fptr1);
    	
    unsigned char seed[SEED_SIZE];
    float num;
    
    if (!read_challenge_file(chalFile, seed, &num)) {
        fclose(chalFile);
        exit(EXIT_FAILURE);
    }
    	
    fclose(chalFile);
    	
    int num_blocks = i*num;
    	
    process_numbers(seed, num_blocks, i, "Challenge_index_PG.txt");
    	    	
    element_set0(add_Zr_points);
    element_set0(add_mu);
    element_set1(pro_sigu);
	
    FILE *file = open_file("Challenge_index_PG.txt", "r");
    	
    int next_int = read_next_integer(file);
    	
    i = 0;
    	
    fptr1 = open_file(arg1, "rb");
    	
    FILE *stat_file = open_file("statistics.txt", "a");
    clock_t startTime, endTime;
    float totalTimeTaken = 0.0;
	
    while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, fptr1)) > 0) {
        i++;
     	read_element_from_file(sig, Sigma_read);
    	 	
     	if(i == next_int) {
    	 	
            startTime = clock();
    	    element_from_hash(bl1, buffer, bytes_read);
            generate_deterministic_v_with_seed(Zr_point1, seed, i);
            element_mul(mu, bl1, Zr_point1);
            element_add(add_mu, add_mu, mu);
            element_pow_zn(j5, sig, Zr_point1);
            element_mul(pro_sigu, pro_sigu, j5);
            element_mul(j1, Be, Zr_point1);
            element_add(add_Zr_points, add_Zr_points, j1);
            endTime = clock();
        		
            totalTimeTaken = (totalTimeTaken + measure_time(startTime, endTime));
        		
            // Now read the next integer from the file for the next iteration
       	    next_int = read_next_integer(file);
        }
    }
        
    fclose(fptr1);
    fclose(file);
        
    startTime = clock();
    element_sub(j2, add_Zr_points, Be);
    element_pow_zn(j3, Pc, j2);
    element_invert(j4, j3);
    element_mul(sigu, pro_sigu, j4);
    endTime = clock();
    	
    totalTimeTaken = (totalTimeTaken + measure_time(startTime, endTime));
    	
    save_element_to_file(add_mu, POP_write);
    save_element_to_file(sigu, POP_write);
        
    fclose(Sigma_read);
    fclose(POP_write);
        
    element_clear(bl1);
    element_clear(Zr_point1);
    element_clear(mu);
    element_clear(add_mu);
    element_clear(add_Zr_points);
    element_clear(sigu);
    element_clear(pro_sigu);
    element_clear(j1);
    element_clear(j2);
    element_clear(j3);
    element_clear(j4);
    element_clear(j5);
    element_clear(sig);
    
    fprintf(stat_file, "Proof Generation Phase Time = %.2f ms\n", totalTimeTaken);
    fclose(stat_file);
}

void proofGen_main(int argc, char **argv) {	
    if(argc < 6){
	fprintf(stderr, "Usage: %s <auditee full private key file> <csp public key file> <input file> <metadata file> <challenge file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	
    element_t Be, Pc, mu, sigu, Pe;
    element_init_Zr(Be, global_params);
    element_init_G1(Pc, global_params);
    element_init_G1(Pe, global_params);
    element_init_Zr(mu, global_params);
    element_init_G1(sigu, global_params);
	
    FILE *privt_key_auditee_file = open_file(argv[1], "rb");
    FILE *pub_key_csp_file = open_file(argv[2], "rb");
	    	
    read_element_from_file(Be, privt_key_auditee_file);
    read_element_from_file(Pc, pub_key_csp_file);
      	
    proofgen(argv[3], argv[4], "POP.bin", argv[5], Be, Pc);
	
    fclose(privt_key_auditee_file);
    fclose(pub_key_csp_file);
    element_clear(Be);
    element_clear(Pc);
    	
    if(debug) {
        printf("Proof Generation Executed Successfully. \nComplete proof is save on file name %s\n\n", "POP.bin");
    }
}

int verifyproof(char **argv, element_t b1, element_t b4, float *totalTimeTaken) {
    if(debug) {
    printf("VERIFY PROOF ALGO INVOKED...\n\n");
    }
    
    element_t Qc, Pc, Pe, mu, sigu, x1, b2, b3, g, g0, wi, Zr_point, j6, pro_wi, j7;
    element_init_G1(Qc, global_params);
    element_init_G1(Pc, global_params);
    element_init_G1(Pe, global_params);
    element_init_G1(sigu, global_params);
    element_init_G1(x1, global_params);
    element_init_G1(g, global_params);
    element_init_G1(g0, global_params);
    element_init_G1(wi, global_params);
    element_init_G1(j6, global_params);
    element_init_G1(j7, global_params);
    element_init_G1(pro_wi, global_params);
    
    element_init_Zr(mu, global_params);
    element_init_Zr(Zr_point, global_params);
    
    element_init_GT(b2, global_params);
    element_init_GT(b3, global_params);

    FILE *pub_key_csp_file = open_file(argv[1], "rb");
    FILE *pub_key_auditee_file = open_file(argv[2], "rb");
    FILE *POP_read = open_file(argv[3], "rb");
    FILE *params_file = open_file(argv[5], "rb");
    FILE *chalFile = open_file(argv[6], "rb");
    
    read_element_from_file(Pc, pub_key_csp_file);
    read_element_from_file(Pe, pub_key_auditee_file);
    read_element_from_file(mu, POP_read);
    read_element_from_file(sigu, POP_read);
    read_element_from_file(g, params_file);
    read_element_from_file(g0, params_file);
    
    H1(Qc, argv[4]);

    int i = 0;
    
    unsigned char seed[SEED_SIZE];
    float num;
    
    read_challenge_file(chalFile, seed, &num);
    
    char *fileName = NULL;
    long long num_blocks;
    read_from_file(argv[7], &fileName, &num_blocks);
    int challenge_blocks = (int)(num_blocks * num);
    
    element_set1(pro_wi);
    
    process_numbers(seed, challenge_blocks, num_blocks, "Challenge_index_VP.txt");
    
    process_and_hash2(challenge_blocks, "Challenge_index_VP.txt", fileName, "H2PV.bin", Pe);
    
    FILE *H2_read = open_file("H2PV.bin", "rb");
    FILE *file = open_file("Challenge_index_VP.txt", "r");
    	
    int next_int = read_next_integer(file);
    
    element_t result;
    element_init_G1(result, global_params);
    char file1[50];
    
    clock_t startTime, endTime;
    
    while (num_blocks > i++) {
        if (i == next_int) {
            read_element_from_file(wi, H2_read);
            
            startTime = clock();
            element_from_hash(result, file1, strlen(file1));
            generate_deterministic_v_with_seed(Zr_point, seed, i);
            element_pow_zn(j6, wi, Zr_point);
            element_mul(pro_wi, pro_wi, j6);
            endTime = clock();
            
    	    *totalTimeTaken = (*totalTimeTaken + measure_time(startTime, endTime));
            
            next_int = read_next_integer(file);
        }
    }
    
    startTime = clock();
    element_pairing(b1, sigu, g);
    element_pow_zn(x1, Qc, mu);
    element_pairing(b2, x1, g0);
    element_mul(j7, pro_wi, Pe);
    element_pairing(b3, j7, Pc);
    element_mul(b4, b2, b3);
    endTime = clock();
    
    *totalTimeTaken = (*totalTimeTaken + measure_time(startTime, endTime));

    fclose(file);
    fclose(chalFile);
    fclose(params_file);
    fclose(POP_read);
    fclose(H2_read);
    fclose(pub_key_csp_file);
    fclose(pub_key_auditee_file);

    element_clear(result);
    element_clear(Pc);
    element_clear(Pe);
    element_clear(mu);
    element_clear(sigu);
    element_clear(x1);
    element_clear(b2);
    element_clear(b3);
    element_clear(g);
    element_clear(g0);
    element_clear(wi);
    element_clear(Zr_point);
    element_clear(j6);
    element_clear(pro_wi);
    element_clear(j7);
}

void verifyProof_main(int argc, char **argv) {
    if (argc < 9) {
        fprintf(stderr, "Usage: %s <csp public key file> <auditee public key file> <POP file> <csp ID> <local params file> <challenge file> <data file identifier> <total number of blocks in data file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    element_t b1, b4;
    
    element_init_GT(b1, global_params);
    element_init_GT(b4, global_params);
    
    FILE *stat_file = open_file("statistics.txt", "a");
    clock_t startTime, endTime;
    float totalTimeTaken = 0.0;
    
    verifyproof(argv, b1, b4, &totalTimeTaken);
    
    startTime = clock();
    if (!element_cmp(b1, b4)) {
        if(lastDebug) {
            printf("\n\nVerification Successfull!\n\n");
        }
        else {
    	    printf("1");
    	}
    }
    else {
        if(lastDebug) {
            printf("\n\nVerification Failed!\n\n");
        }
        else {
    	    printf("0");
    	}
    }
    endTime = clock();
    
    totalTimeTaken = (totalTimeTaken + measure_time(startTime, endTime));

    element_clear(b1);
    element_clear(b4);
    
    fprintf(stat_file, "Verify Proof Phase Time = %.2f ms\n", totalTimeTaken);
    fclose(stat_file);

    if(debug) {
    printf("Verify Proof Executed Successfully.");
    }
}

int main(int argc, char **argv) {
        myPBC_Initialize(argv[2]);
        
        if (strcmp(argv[1], "setup") == 0){
		setup_main();
	}
	else if (strcmp(argv[1], "partialKeyGen") == 0){
		partialKeyGen_main( argc, (argv+2) );
	}
	else if (strcmp(argv[1], "fullKeyGen") == 0){
		fullKeyGen_main( argc, (argv+2) );
	}
	else if (strcmp(argv[1], "tagGen") == 0){
		tagGen_main( argc, (argv+2) );
	}
	else if (strcmp(argv[1], "chalGen") == 0){
		chalGen_main( argc, (argv+2) );
	}
	else if (strcmp(argv[1], "proofGen") == 0){
		proofGen_main( argc, (argv+2) );
	}
	else if (strcmp(argv[1], "verifyProof") == 0){
		verifyProof_main( argc, (argv+2) );
	}
	else{
		printf("Incorrect Command\n");
	}
        
        pairing_clear(global_params);
        
	return 0;
}

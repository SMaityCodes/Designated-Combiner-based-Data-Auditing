/*  USAGE: ./<object_name> <param_file> <mode> <n>
    where, mode = 1 for printing the time in 'ms' mode = 0 for priting in no. of CPU-Cycles.
    For eaxmple:   ./PBC_time a.param 1 1000
    or, ./PBC_time a.param 0 1000
*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h> // for intptr_t
#include <pbc/pbc.h>
#include <pbc/pbc_test.h>

#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	if(argc < 3){
		cerr << "Error! Incorrect Usage: ./<object_name> <a.param> <mode>" << endl;
		exit(1);
	}
	size_t mode;
	mode = stoi(argv[2]);
	
	bool debug = false;
	
	pairing_t params;
	pbc_demo_pairing_init(params, argc, argv);
	size_t n; 
	if(debug) n = 1;
	else n = stoi(argv[3]);; // accuracy vs. cost
	
	
	clock_t startTime1, endTime1;
    	double elapsed_time1;
    	double avgTimeTaken1, totalTimeTaken1 = 0.0;
    	
    	clock_t startTime2, endTime2;
    	double elapsed_time2;
    	double avgTimeTaken2, totalTimeTaken2 = 0.0;
    	
    	clock_t startTime3, endTime3;
    	double elapsed_time3;
    	double avgTimeTaken3, totalTimeTaken3 = 0.0;

	clock_t startTime4, endTime4, startTime5, endTime5, startTime6, endTime6, startTime7, endTime7, startTime8, endTime8, startTime9, endTime9, startTime10, endTime10, startTime11, endTime11;
	double elapsed_time4, elapsed_time5, elapsed_time6, elapsed_time7, elapsed_time8, elapsed_time9, elapsed_time10, elapsed_time11;
	double avgTimeTaken4, avgTimeTaken5, avgTimeTaken6, avgTimeTaken7, totalTimeTaken4 = 0.0, totalTimeTaken5 = 0.0, totalTimeTaken6 = 0.0, totalTimeTaken7 = 0.0;
	double avgTimeTaken8, avgTimeTaken9, avgTimeTaken10, avgTimeTaken11, totalTimeTaken8 = 0.0, totalTimeTaken9 = 0.0, totalTimeTaken10 = 0.0, totalTimeTaken11 = 0.0;
	
	clock_t startTime12, endTime12, startTime13, endTime13, startTime14, endTime14, startTime15, endTime15, startTime16, endTime16, startTime17, endTime17, startTime18, endTime18;
    	double elapsed_time12, elapsed_time13, elapsed_time14, elapsed_time15, elapsed_time16, elapsed_time17, elapsed_time18;
    	double avgTimeTaken12, avgTimeTaken13, avgTimeTaken14, avgTimeTaken15, avgTimeTaken16, avgTimeTaken17, avgTimeTaken18;
    	double totalTimeTaken12 = 0.0, totalTimeTaken13 = 0.0, totalTimeTaken14 = 0.0, totalTimeTaken15 = 0.0, totalTimeTaken16 = 0.0, totalTimeTaken17 = 0.0, totalTimeTaken18 = 0.0;
    	
    	
	element_t P1, P2, P3, P4, P5, P6, X1, X2, X3, d1, d2, d3;
  
	element_init_G1(P1, params);
	element_init_G1(P2, params);
	element_init_G1(P3, params);
  
	element_init_G2(P4, params);
	element_init_G2(P5, params);
	element_init_G2(P6, params);
	
	element_init_GT(X1, params);
	element_init_GT(X2, params);
	element_init_GT(X3, params);
	
	element_init_Zr(d1, params);
	element_init_Zr(d2, params);
	element_init_Zr(d3, params);
	
    	for(size_t i = 0; i < n; i++){
    	        cout << endl << "Processing for : " << i << endl;
    	
    		element_random(P1);
		element_random(P2);
		element_random(d1);
		element_random(d2);
		element_random(P4);
		element_random(P5);
    		element_random(X1);
		element_random(X2);
		
		/////////////////                      Calculations in G1        /////////////////////////////////////////
		
		
    		startTime1 = clock();
    		
      		element_add(P3, P1, P2);
      		
    		endTime1 = clock();
    		if(mode) elapsed_time1 = static_cast<double>(endTime1 - startTime1)/CLOCKS_PER_SEC*1000;
    		else elapsed_time1 = static_cast<double>(endTime1 - startTime1);
    		totalTimeTaken1 = (totalTimeTaken1 + elapsed_time1);
    		
    		startTime2 = clock();
    		
      		element_sub(P3, P1, P2);
      		
    		endTime2 = clock();
    		if(mode) elapsed_time2 = static_cast<double>(endTime2 - startTime2)/CLOCKS_PER_SEC*1000;
    		else elapsed_time2 = static_cast<double>(endTime2 - startTime2);
    		totalTimeTaken2 = (totalTimeTaken2 + elapsed_time2);
    		
    		startTime3 = clock();
    		
      		element_mul_zn(P2, P1, d1);
      		
    		endTime3 = clock();
    		if(mode) elapsed_time3 = static_cast<double>(endTime3 - startTime3)/CLOCKS_PER_SEC*1000;
    		else elapsed_time3 = static_cast<double>(endTime3 - startTime3);
    		totalTimeTaken3 = (totalTimeTaken3 + elapsed_time3);
    		
    		startTime4 = clock();
    		
      		element_invert(P2, P1);
      		
    		endTime4 = clock();
    		if(mode) elapsed_time4 = static_cast<double>(endTime4 - startTime4)/CLOCKS_PER_SEC*1000;
    		else elapsed_time4 = static_cast<double>(endTime4 - startTime4);
    		totalTimeTaken4 = (totalTimeTaken4 + elapsed_time4);
    		
    		
    		/////////////////                      Calculations in G2        /////////////////////////////////////////
    		
    		
    		startTime5 = clock();
    		
      		element_add(P6, P4, P5);
      		
    		endTime5 = clock();
    		if(mode) elapsed_time5 = static_cast<double>(endTime5 - startTime5)/CLOCKS_PER_SEC*1000;
    		else elapsed_time5 = static_cast<double>(endTime5 - startTime5);
    		totalTimeTaken5 = (totalTimeTaken5 + elapsed_time5);
    		
    		startTime6 = clock();
    		
      		element_sub(P6, P4, P5);
      		
    		endTime6 = clock();
    		if(mode) elapsed_time6 = static_cast<double>(endTime6 - startTime6)/CLOCKS_PER_SEC*1000;
    		else elapsed_time6 = static_cast<double>(endTime6 - startTime6);
    		totalTimeTaken6 = (totalTimeTaken6 + elapsed_time6);
    		
    		startTime7 = clock();
    		
      		element_mul_zn(P5, P4, d1);
      		
    		endTime7 = clock();
    		if(mode) elapsed_time7 = static_cast<double>(endTime7 - startTime7)/CLOCKS_PER_SEC*1000;
    		else elapsed_time7 = static_cast<double>(endTime7 - startTime7);
    		totalTimeTaken7 = (totalTimeTaken7 + elapsed_time7);
    		
    		startTime8 = clock();
    		
      		element_invert(P5, P4);
      		
    		endTime8 = clock();
    		if(mode) elapsed_time8 = static_cast<double>(endTime8 - startTime8)/CLOCKS_PER_SEC*1000;
    		else elapsed_time8 = static_cast<double>(endTime8 - startTime8);
    		totalTimeTaken8 = (totalTimeTaken8 + elapsed_time8);
    		
    		/////////////////                      Calculations in GT        /////////////////////////////////////////
    		
    	
    		startTime9 = clock();
    		
      		element_mul(X3, X1, X2);
      		
    		endTime9 = clock();
    		if(mode) elapsed_time9 = static_cast<double>(endTime9 - startTime9)/CLOCKS_PER_SEC*1000;
    		else elapsed_time9 = static_cast<double>(endTime9 - startTime9);
    		totalTimeTaken9 = (totalTimeTaken9 + elapsed_time9);
    		
    		startTime10 = clock();
    		
      		element_invert(X2, X1);
      		
    		endTime10 = clock();
    		if(mode) elapsed_time10 = static_cast<double>(endTime10 - startTime10)/CLOCKS_PER_SEC*1000;
    		else elapsed_time10 = static_cast<double>(endTime10 - startTime10);
    		totalTimeTaken10 = (totalTimeTaken10 + elapsed_time10);
    		
    		startTime11 = clock();
    		
      		element_pow_zn(X2, X1, d1);
      		
    		endTime11 = clock();
    		if(mode) elapsed_time11 = static_cast<double>(endTime11 - startTime11)/CLOCKS_PER_SEC*1000;
    		else elapsed_time11 = static_cast<double>(endTime11 - startTime11);
    		totalTimeTaken11 = (totalTimeTaken11 + elapsed_time11);
    		
    		/////////////////                      Calculations in Zr        /////////////////////////////////////////
    		
    		
    		if(debug){
    			element_set_si(d1, 5);
    			element_set_si(d2, 2);
    			element_printf("d1= %B\n", d1);
    			element_printf("d2= %B\n", d2);
    		}
    		
    		startTime12 = clock();
    		
      		element_add(d3, d1, d2);
      		
    		endTime12 = clock();
    		if(mode) elapsed_time12 = static_cast<double>(endTime12 - startTime12)/CLOCKS_PER_SEC*1000;
    		else elapsed_time12 = static_cast<double>(endTime12 - startTime12);
    		totalTimeTaken12 = (totalTimeTaken12 + elapsed_time12);
    		if(debug) element_printf("Result of addition: %B\n", d3);
    		
    		startTime13 = clock();
    		
      		element_sub(d3, d1, d2);
      		
    		endTime13 = clock();
    		if(mode) elapsed_time13 = static_cast<double>(endTime13 - startTime13)/CLOCKS_PER_SEC*1000;
    		else elapsed_time13 = static_cast<double>(endTime13 - startTime13);
    		totalTimeTaken13 = (totalTimeTaken13 + elapsed_time13);
    		if(debug) element_printf("Result of subtraction: %B\n", d3);
    		
    		
    		startTime14 = clock();
    		
      		element_mul(d3, d1, d2);
      		
    		endTime14 = clock();
    		if(mode) elapsed_time14 = static_cast<double>(endTime14 - startTime14)/CLOCKS_PER_SEC*1000;
    		else elapsed_time14 = static_cast<double>(endTime14 - startTime14);
    		totalTimeTaken14 = (totalTimeTaken14 + elapsed_time14);
    		if(debug) element_printf("Result of multiplication: %B\n", d3);
    		
    		startTime15 = clock();
    		
      		element_div(d3, d1, d2);
      		
    		endTime15 = clock();
    		if(mode) elapsed_time15 = static_cast<double>(endTime15 - startTime15)/CLOCKS_PER_SEC*1000;
    		else elapsed_time15 = static_cast<double>(endTime15 - startTime15);
    		totalTimeTaken15 = (totalTimeTaken15 + elapsed_time15);
    		if(debug) element_printf("Result of division: %B\n", d3);
    		
    		startTime16 = clock();
    		
      		element_invert(d3, d2);
      		
    		endTime16 = clock();
    		if(mode) elapsed_time16 = static_cast<double>(endTime16 - startTime16)/CLOCKS_PER_SEC*1000;
    		else elapsed_time16 = static_cast<double>(endTime16 - startTime16);
    		totalTimeTaken16 = (totalTimeTaken16 + elapsed_time16);
    		if(debug){
    			element_printf("Result of d2-inverse: %B\n", d3);
    			element_mul(d3, d1, d3);
    			element_printf("Result of d1*d2inverse: %B\n", d3);
    		}
    		
    		startTime17 = clock();
    		
      		element_pow_zn(d3, d1, d2); 
      		
    		endTime17 = clock();
    		if(mode) elapsed_time17 = static_cast<double>(endTime17 - startTime17)/CLOCKS_PER_SEC*1000;
    		else elapsed_time17 = static_cast<double>(endTime17 - startTime17);
    		totalTimeTaken17 = (totalTimeTaken17 + elapsed_time17);
    		if(debug) element_printf("Result of exponentiation: %B\n", d3);
    		
    		
    		/////////////////                      BILINEAR PAIRING OPERATION       /////////////////////////////////////////
    		
    		
    		startTime18 = clock();
    		
      		element_pairing(X3, P1, P2);
      		
    		endTime18 = clock();
    		if(mode) elapsed_time18 = static_cast<double>(endTime18 - startTime18)/CLOCKS_PER_SEC*1000;
    		else elapsed_time18 = static_cast<double>(endTime18 - startTime18);
    		totalTimeTaken18 = (totalTimeTaken18 + elapsed_time18);
    	}
    	
    	 
    	avgTimeTaken1 = (totalTimeTaken1 / n);
    	avgTimeTaken2 = (totalTimeTaken2 / n);
    	avgTimeTaken3 = (totalTimeTaken3 / n);
    	avgTimeTaken4 = (totalTimeTaken4 / n);
    	avgTimeTaken5 = (totalTimeTaken5 / n);
    	avgTimeTaken6 = (totalTimeTaken6 / n);
    	avgTimeTaken7 = (totalTimeTaken7 / n);
    	avgTimeTaken8 = (totalTimeTaken8 / n);
    	avgTimeTaken9 = (totalTimeTaken9 / n);
    	avgTimeTaken10 = (totalTimeTaken10 / n);
    	avgTimeTaken11 = (totalTimeTaken11 / n);
    	avgTimeTaken12 = (totalTimeTaken12 / n);
    	avgTimeTaken13 = (totalTimeTaken13 / n);
    	avgTimeTaken14 = (totalTimeTaken14 / n);
    	avgTimeTaken15 = (totalTimeTaken15 / n);
    	avgTimeTaken16 = (totalTimeTaken16 / n);
    	avgTimeTaken17 = (totalTimeTaken17 / n);
    	avgTimeTaken18 = (totalTimeTaken18 / n);
    	
    	ofstream file("Time.txt");
    	
    	// Check if the file opened successfully
        if (!file) {
          cerr << "Error opening file!" << std::endl;
          return 1;
        }
    	
    	cout << endl << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;
    	file << endl << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;
    	
    	cout << endl << "n = " << n << endl;
    	file << endl << "n = " << n << endl;
    	
	if(mode){
		cout << endl << "TIME REPORT IN G1 ............." << endl;
		file << endl << "TIME REPORT IN G1 ............." << endl;
		cout << endl << "Cost of Addition in G1 = " << avgTimeTaken1 << " ms" << endl;
		file << endl << "Cost of Addition in G1 = " << avgTimeTaken1 << " ms" << endl;
		cout << endl << "Cost of Subtraction in G1 = " << avgTimeTaken2 << " ms" << endl;
		file << endl << "Cost of Subtraction in G1 = " << avgTimeTaken2 << " ms" << endl;
		cout << endl << "Cost of scalar-Multiplication in G1 = " << avgTimeTaken3 << " ms" << endl;
		file << endl << "Cost of scalar-Multiplication in G1 = " << avgTimeTaken3 << " ms" << endl;
		cout << endl << "Cost of Point Negation in G1 = " << avgTimeTaken4 << " ms" << endl;
		file << endl << "Cost of Point Negation in G1 = " << avgTimeTaken4 << " ms" << endl;
		
		cout << endl << "TIME REPORT IN G2 ............." << endl;
		file << endl << "TIME REPORT IN G2 ............." << endl;
		cout << endl << "Cost of Addition in G2 = " << avgTimeTaken5 << " ms" << endl;
		file << endl << "Cost of Addition in G2 = " << avgTimeTaken5 << " ms" << endl;
		cout << endl << "Cost of Subtraction in G2 = " << avgTimeTaken6 << " ms" << endl;
		file << endl << "Cost of Subtraction in G2 = " << avgTimeTaken6 << " ms" << endl;
		cout << endl << "Cost of scalar-Multiplication in G2 = " << avgTimeTaken7 << " ms" << endl;
		file << endl << "Cost of scalar-Multiplication in G2 = " << avgTimeTaken7 << " ms" << endl;
		cout << endl << "Cost of Point Negation in G2 = " << avgTimeTaken8 << " ms" << endl;
		file << endl << "Cost of Point Negation in G2 = " << avgTimeTaken8 << " ms" << endl;
		
		cout << endl << "TIME REPORT IN GT ............." << endl;
		file << endl << "TIME REPORT IN GT ............." << endl;
		cout << endl << "Cost of Multiplication in GT = " << avgTimeTaken9 << " ms" << endl;
		file << endl << "Cost of Multiplication in GT = " << avgTimeTaken9 << " ms" << endl;
		cout << endl << "Cost of Inverse in GT = " << avgTimeTaken10 << " ms" << endl;
		file << endl << "Cost of Inverse in GT = " << avgTimeTaken10 << " ms" << endl;
		cout << endl << "Cost of Exponentiation in GT = " << avgTimeTaken11 << " ms" << endl;
		file << endl << "Cost of Exponentiation in GT = " << avgTimeTaken11 << " ms" << endl;
		
		cout << endl << "TIME REPORT IN Zr ............." << endl;
		file << endl << "TIME REPORT IN Zr ............." << endl;
		cout << endl << "Cost of Addition in Zr = " << avgTimeTaken12 << " ms" << endl;
		file << endl << "Cost of Addition in Zr = " << avgTimeTaken12 << " ms" << endl;
		cout << endl << "Cost of Subtraction in Zr = " << avgTimeTaken13 << " ms" << endl;
		file << endl << "Cost of Subtraction in Zr = " << avgTimeTaken13 << " ms" << endl;
		cout << endl << "Cost of Multiplication in Zr = " << avgTimeTaken14 << " ms" << endl;
		file << endl << "Cost of Multiplication in Zr = " << avgTimeTaken14 << " ms" << endl;
		cout << endl << "Cost of Division in Zr = " << avgTimeTaken15 << " ms" << endl;
		file << endl << "Cost of Division in Zr = " << avgTimeTaken15 << " ms" << endl;
		cout << endl << "Cost of Multiplicative-Inverse in Zr = " << avgTimeTaken16 << " ms" << endl;
		file << endl << "Cost of Multiplicative-Inverse in Zr = " << avgTimeTaken16 << " ms" << endl;
		cout << endl << "Cost of Exponentiation in Zr = " << avgTimeTaken17 << " ms" << endl;
		file << endl << "Cost of Exponentiation in Zr = " << avgTimeTaken17 << " ms" << endl;
		
		cout << endl << "BILINEAR PAIRING....." << endl;
		file << endl << "BILINEAR PAIRING....." << endl;
		cout << "Pairing Cost = " << avgTimeTaken18 << " ms"<< endl;
		file << "Pairing Cost = " << avgTimeTaken18 << " ms"<< endl;
		
	}
	else{
		cout << "TIME REPORT IN G1 ............." << endl;
		cout << endl << "Cost of Addition in G1 = " << avgTimeTaken1 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Subtraction in G1 = " << avgTimeTaken2 << " CPU-Cycles" << endl;
		cout << endl << "Cost of scalar-Multiplication in G1 = " << avgTimeTaken3 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Point Negation in G1 = " << avgTimeTaken4 << " CPU-Cycles" << endl;
		
		cout << endl << "TIME REPORT IN G2 ............." << endl;
		cout << endl << "Cost of Addition in G2 = " << avgTimeTaken5 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Subtraction in G2 = " << avgTimeTaken6 << " CPU-Cycles" << endl;
		cout << endl << "Cost of scalar-Multiplication in G2 = " << avgTimeTaken7 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Point Negation in G2 = " << avgTimeTaken8 << " CPU-Cycles" << endl;
		
		cout << endl << "TIME REPORT IN GT ............." << endl;
		cout << endl << "Cost of Multiplication in GT = " << avgTimeTaken9 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Inverse in GT = " << avgTimeTaken10 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Exponentiation in GT = " << avgTimeTaken11 << " CPU-Cycles" << endl;
		
		cout << endl << "TIME REPORT IN Zr ............." << endl;
		cout << endl << "Cost of Addition in Zr = " << avgTimeTaken12 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Subtraction in Zr = " << avgTimeTaken13 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Multiplication in Zr = " << avgTimeTaken14 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Division in Zr = " << avgTimeTaken15 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Multiplicative-Inverse in Zr = " << avgTimeTaken16 << " CPU-Cycles" << endl;
		cout << endl << "Cost of Exponentiation in Zr = " << avgTimeTaken17 << " CPU-Cycles" << endl;
		
		cout << endl << "BILINEAR PAIRING....." << endl;
		cout << "Pairing Cost = " << avgTimeTaken18 << " CPU-Cycles"<< endl;
		
	}
	
	// Close the file
        file.close();
	
	return 0;
}

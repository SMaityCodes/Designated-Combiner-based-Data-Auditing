// ./fileCompare <file_name_1> <file_name_2> <file_block_size>

#include"dataAuditUtils.h"

void PrintBuffer(char * buff, size_t length){
	char c;
	for(size_t i = 0; i < length; i++){
		//cout << hex;
		//cout << dec;
		c = buff[i];
		cout <<  (int) c << " ";
	}
	cout << endl;
	cout << dec;
}

// Returns 1 if the blocks are same ; 0 otherwise
int CompareBlocks(char * buff1, char * buff2, size_t length){
	for(size_t i = 0; i < length; i++){
		if(buff1[i] != buff2[i]){
			return 0;
		}
	}
	return 1;
}


int main(int argc, char **argv) {
	if(argc < 4){
		printf("Error: Give two file names. %d !!!\n", argc);
		exit(1);
    	}
    	
    	
    	size_t blocksize = stoi(argv[3]);
    	
    	MyFileClass file1(argv[1], blocksize);  
    	MyFileClass file2(argv[2], blocksize); 
    	
    	size_t fileSize1 = file1.getSize();
    	size_t fileSize2 = file2.getSize();
    	size_t n = file1.getBlockCount();
    	//size_t s = file1.s;
    	
    	
    	if(fileSize1 != fileSize2){
    		cerr << "Cannot Compare. Given Files are of Different Sizes. " << argv[1] << endl;
        	exit(1);
    	}
    	
    	
    	
    	
    	// Fetch the blocks of File-1 one-by-one
    	void * buffer1 =  malloc(blocksize);
    	void * buffer2 =  malloc(blocksize);
    	void * buffer3 =  malloc(blocksize);
    	vector<size_t> indices;
    	double proportion;
    	
    	for(size_t i = 0; i < n; i++){
    		file1.GetBinaryBlock((char *) buffer1, i);
    		file2.GetBinaryBlock((char *) buffer2, i);
    		bitwiseXOR((unsigned char *) buffer3, (const unsigned char *) buffer1, (const unsigned char *) buffer2, blocksize);
    		
    		/*cout << "Block-" << i << " of File-1 :-" << endl;
    		PrintBuffer((char *) buffer1, blocksize); 
    		cout << "Block-" << i << " of File-2 :-" << endl;
    		PrintBuffer((char *) buffer2, blocksize); 
    		cout << "XOR-ed :" << endl;
    		PrintBuffer((char *) buffer3, blocksize);*/
    		
    		if(CompareBlocks((char *) buffer1 , (char *) buffer2, blocksize)){
    			//cout << "Block-" << i << ": SAME" << endl;	
    		}
    		else{
    			//cout << "Block-" << i << ": NOT SAME." << endl; 
    			indices.push_back(i);
    		}
    		
    	}
    	
    	proportion = ((indices.size()) / (n*1.0));
    	
    	// Display Details of the files.
    	
    	//cout<< "Details of Input Files:-" << endl;
    	//cout << "Filesize = " << fileSize1 << " bytes" << endl;
    	
    	cout << "Number of blocks(n) = "<< n << endl;
    	
    	if(1) cout << "Total no. of Mismatching blocks = " << indices.size() <<endl;
    	if(1) cout << "Proportion of Mismatching blocks = " << proportion <<endl;
    	if(1){
    		cout << "List of Mismatching Indices of the two files:-" << endl;
    		PrintVector(indices);
    	}
    	file1.close();
    	file2.close();
    	cout << "File Comparing Program terminates Successfully.===================================================" << endl;
	return 0;
}

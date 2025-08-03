// ./Attack <file_name> <attack_Percentage> <file_Block_size>

#include"dataAuditUtils.h"

int main(int argc, char **argv){
	if(debug) cout << endl<< "  ==============================    Attacker Executing    =========================================   " << endl;
	
	if(argc < 4){
		printf("Error: Please Enter Correct Execution Command %d !!!\n", argc);
		exit(1);
    	}
    	double attackPrcnt = stod(argv[2]);
    	size_t blocksize =  stoi(argv[3]);
    	size_t indx, elem;
    	string seed_str = GetRandomString(5);
    	
    	MyFileClass file(argv[1], blocksize); 
    	
    	size_t n = file.getBlockCount();
    	size_t c = (n*attackPrcnt);
    	void * buffer =  malloc(blocksize);
    	getRandomZn randZn(seed_str, n);
    	vector<size_t> indices;
    	
    	if(debug){
    		cout << "Total No. of Blocks  = " << n << endl;
    		cout << "Attack Percentage = " << attackPrcnt << endl;
    		cout << "No. of Blocks to Attack = " << c << endl;
    	}
    	
    	getRandomZn randVal(seed_str, 9999);
    	
    	memset(buffer, randVal.getElementUnique(), blocksize);
    	
    	
    	for(size_t i = 0; i < c; i++){
    		elem = randZn.getElementUnique();
    		auto position = std::lower_bound(indices.begin(), indices.end(), elem);
    		indices.insert(position, elem);
    	}
    	
    	
    	for(size_t i = 0; i < c; i++){
    		// Get the i'th index to attack 
    		indx = indices[i];
    		file.WriteBinaryBlock((char *) buffer, indx);
    		//cout << "Modifying Block-" << indx << endl;
    	}
    	if(debug){
    		cout << "List of Attacked Indices:-" << endl;
    		PrintVector(indices);
    	}
    	
    	file.close();
    	cout << "=====================================     Attacker Program terminates Successfully.==================================================" << endl;
	return 0;
}

# Define the input file as a variable
INPUT_FILE := in.ods
PARAM_FILE := a.param

all: dataAudit

dataAudit:
	@echo "Compiling our data auditing software..."
	gcc -o dataAudit dataAudit.c -lgmp -lpbc

runall: runSetup runPartialKeyGen runFullKeyGen runTagGen runChalGen runProofGen runVerifyProof
	
runSetup:
	./dataAudit setup $(PARAM_FILE)
	
runPartialKeyGen:
	./dataAudit partialKeyGen $(PARAM_FILE) MSK.bin soumyadev@iiita.ac.in
	./dataAudit partialKeyGen $(PARAM_FILE) MSK.bin junaid@iiita.ac.in
	
runFullKeyGen:
	./dataAudit fullKeyGen $(PARAM_FILE) localParams.bin soumyadev_partial_private_key.bin soumyadev@iiita.ac.in
	./dataAudit fullKeyGen $(PARAM_FILE) localParams.bin junaid_partial_private_key.bin junaid@iiita.ac.in
	
runTagGen:
	./dataAudit tagGen $(PARAM_FILE) soumyadev_full_private_key.bin junaid_public_key.bin $(INPUT_FILE)
	
runChalGen:
	./dataAudit chalGen $(PARAM_FILE) 0.04
	
runProofGen:
	./dataAudit proofGen $(PARAM_FILE) junaid_full_private_key.bin soumyadev_public_key.bin $(INPUT_FILE) sigma.bin chal_file.txt
	
runVerifyProof:
	./dataAudit verifyProof $(PARAM_FILE) soumyadev_public_key.bin junaid_public_key.bin POP.bin soumyadev@iiita.ac.in localParams.bin chal_file.txt file_info.txt

clean:
	@echo "Remove all optional files..."
	rm dataAudit MSK.bin localParams.bin soumyadev_partial_private_key.bin soumyadev_full_private_key.bin soumyadev_public_key.bin junaid_partial_private_key.bin junaid_full_private_key.bin junaid_public_key.bin sigma.bin POP.bin H2TG.bin H2PV.bin integer.txt Challenge_index_VP.txt Challenge_index_PG.txt chal_file.txt file_info.txt

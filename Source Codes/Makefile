# Define the input file as a variable
INPUT_FILE := in.ods
PARAM_FILE := a.param

all: dataAudit

dataAudit:
	@echo "Compiling our data auditing software..."
	gcc -o dataAudit dataAudit.c -lgmp -lpbc

runall: runSetup runPartialKeyGen runFullKeyGen runTagGen runChalGen runProofGen runVerifyProof
	
runSetup:
	@./dataAudit setup $(PARAM_FILE)
	@echo "Setup Done..."

runPartialKeyGen:
	@./dataAudit partialKeyGen $(PARAM_FILE) MSK.bin csp@iiita.ac.in
	@./dataAudit partialKeyGen $(PARAM_FILE) MSK.bin dc@iiita.ac.in

runFullKeyGen:
	@./dataAudit fullKeyGen $(PARAM_FILE) localParams.bin csp_partial_private_key.bin csp@iiita.ac.in
	@./dataAudit fullKeyGen $(PARAM_FILE) localParams.bin dc_partial_private_key.bin dc@iiita.ac.in
	@echo "Key Pair Generated..."

runTagGen:
	@./dataAudit tagGen $(PARAM_FILE) csp_full_private_key.bin dc_public_key.bin $(INPUT_FILE)
	@echo "Tag File Generated..."
	@rm H2TG.bin integer.txt dc_partial_private_key.bin csp_partial_private_key.bin MSK.bin
	
runChalGen:
	@./dataAudit chalGen $(PARAM_FILE) 0.04
	@echo "Challenge File Generated..."

runProofGen:
	@./dataAudit proofGen $(PARAM_FILE) dc_full_private_key.bin csp_public_key.bin $(INPUT_FILE) sigma.bin chal_file.txt
	@echo "POP File Generated..."
	@rm Challenge_index_PG.txt

runVerifyProof:
	@./dataAudit verifyProof $(PARAM_FILE) csp_public_key.bin dc_public_key.bin POP.bin csp@iiita.ac.in localParams.bin chal_file.txt file_info.txt
	@echo "-Verification Result"
	@rm H2PV.bin Challenge_index_VP.txt

clean:
	@echo "Remove all optional files..."
	rm dataAudit localParams.bin csp_full_private_key.bin csp_public_key.bin dc_full_private_key.bin dc_public_key.bin sigma.bin POP.bin chal_file.txt file_info.txt

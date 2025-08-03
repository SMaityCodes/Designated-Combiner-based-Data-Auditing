# Define the input file as a variable
INPUT_FILE := in.ods
PARAM_FILE := a.param

all: runSetup runPartialKeyGen runFullKeyGen runTagGen

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

clean:
	@echo "Remove all optional files..."
	@rm -f MSK.bin localParams.bin csp_full_private_key.bin csp_public_key.bin dc_full_private_key.bin dc_public_key.bin sigma.bin file_info.txt

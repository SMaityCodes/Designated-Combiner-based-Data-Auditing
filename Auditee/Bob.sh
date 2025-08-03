# THIS IS BOB

#!/bin/bash
MY_PORT=22262
ALICE_IP="172.21.11.149"
ALICE_PORT=22261

# Define the input file as a variable
INPUT_FILE := in.ods
PARAM_FILE := a.param

trap "exit" SIGINT


while true; do
	# Wait for seed.bin and chal_file.txt from Alice
	socat - TCP-LISTEN:$MY_PORT,reuseaddr > chal_file.txt

	# Run ProofGen: takes chal_file.txt and other necessary files as input and generates POP.bin as output
	./dataAudit proofGen $(PARAM_FILE) dc_full_private_key.bin csp_public_key.bin $(INPUT_FILE) sigma.bin chal_file.txt

	# Send POP.bin to Alice
	cat POP.bin | socat - TCP:$ALICE_IP:$ALICE_PORT

done

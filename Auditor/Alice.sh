# THIS IS ALICE

#!/bin/bash
MY_PORT=22261
BOB_IP="172.17.15.9"
BOB_PORT=22262
counter=1

PARAM_FILE := a.param

trap "exit" SIGINT


while true; do
	echo "Auditing instance $counter"
	
	# Run chalGen: generates chal_file.txt as output
	./dataAudit chalGen $(PARAM_FILE) 0.04

	# Send chal_per.txt and seed.bin to Bob
	cat chal_file.txt | socat - TCP:$BOB_IP:$BOB_PORT

	# Wait for POP.bin from Bob
	socat - TCP-LISTEN:$MY_PORT,reuseaddr > POP.bin

	# Run verifyProof: takes POP.bin and other necessary files as input and prints the result
	res=$(./dataAudit verifyProof $(PARAM_FILE) csp_public_key.bin dc_public_key.bin POP.bin csp@iiita.ac.in localParams.bin chal_file.txt file_info.txt)
	if [ $res -eq 1 ]; then
		echo "VERIFICATION SUUCCESSFUL !"
		paplay Success_Msg.ogg
	else
		echo "VERIFICATION FAILED !  DATA IS CORRUPUTED !"
		paplay Failure_msg.ogg
	fi
	
	sleep 1
	counter=$((counter + 1))

done

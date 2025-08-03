# FOG-CPS Data Auditing

## 📁 Project Structure

```
FOG-CPS Data Auditing
├── Aditor/              
│   ├── Alice.sh            # SHELL script for auditor's protocol    
│   ├── Configuration.txt   # Protocol configuration and debug parameters
│   ├── DataAudit           # [executable] the main data-auditing software
│   ├── Failure_Msg.ogg     # supporting audio file 
│   └── Success_Msg.ogg     # supporting audio file
│
├── Auditee/              
│   ├── 15MBData.csv        # A sample data-file 
│   ├── 15MBData2.csv       # Copy of the sample data-file
│   ├── Bob.sh              # SHELL script for auditee's protocol 
│   ├── Configuration.txt   # Protocol configuration and debug parameters
│   ├── DataAudit           # [executable] the main data-auditing software
│   ├── Attack              # [executable] the attack-software
│   └── fileCompare         # [executable] software for comparing the difference between two data-files
│
├── CSP-Admin/              
│   ├── Configuration.txt   # Protocol configuration and debug parameters
│   ├── DataAudit           # [executable] the main data-auditing software
│   └── SetupTagGen.sh      # SHELL script for admin's protocol
│
├── Experimentation Scripts/ # SHELL scripts for accuracy and execution-time measurement
│   ├── Accuracy Testing/
│   │   └── ChallengeResponse.sh
│   └── Time Measurement/
│       ├── MeasureProofGenTime.sh
│       ├── MeasureProofVerifyTime.sh
│       ├── MeasureSetupTime.sh
│       ├── MeasureTagGenTime.sh
│       └── SetupTagGen.sh
│
└── Source Codes/            # All source codes
    ├── DataAudit.cpp
    ├── dataAuditutils.h
    ├── dataAuditv2.h
    ├── generalUtility.h
    ├── Attack.cpp
    ├── fileCompare.cpp    
    └── Makefile

```
**Note**: the copies of the `Configuration.txt` file in all three directories - "Auditor", "Auditee" and "CSP-Admin" must be consistent.

## ⚙️ Prerequisite:-

- UNIX OS (Ubuntu)
- GNU Compiler Collection (GCC)
- Crypto++ Library

In order to install Crypto++, run the following:-
```
sudo apt-cache pkgnames | grep -i crypto++
sudo apt-get install libcrypto++-utils
sudo apt-get install libcrypto++8
sudo apt-get install libcrypto++-dev
sudo apt-get install libcrypto++-doc
```

## 🚀 How to Use This Repository?
You can test it in three different systems connected by a network - one system working as the cloud (Admin), one acting as the auditor and the other one acting as the auditee. However, you can also test it on a single system (in different terminals). Our files are written assuming the testing to be done on same system. Make the changes accordingly if you test on different systems.

- clone the repository
- copy the "CSP-Admin" directory into the cloud (Admin) system
- copy the "Aditor" directory into the aditor system
- copy the "Auditee" directory into the auditee system

### Configure the Aditor, Auditee and the CSP-Admin

- Go inside "CSP-Admin" dir:-
    - make sure that the path and filename of the data-file (file to be audited) is properly mentioned in the  `SetupTagGen.sh` script
    - run the following command on a terminal opened inside the dir:-
      ```
      chmod +x SetupTagGen.sh DataAudit
      ./SetupTagGen.sh
      ```
    - the above will generate the admin's secret-key *alpha*, the public parameters *params* and the *metadata* of the data-file using *alpha* and *params*
    - copy the params file (`Params.bin`) to both "Aditor" and "Auditee" directories
    - copy only the *metadata* file (`metaData.bin`) to "Aditor" dir only
- Go inside "Auditor" dir:-
    - set the Auditee’s IP address inside `Alice.sh` (set `127.0.0.1` for localhost)
    - run the following command on a terminal opened inside the dir:-
      ```
      chmod +x Alice.sh DataAudit
      ```
- Go inside "Auditee" dir:-
    - set the Auditor’s IP address inside `Bob.sh` (set `127.0.0.1` for localhost)
    - run the following command on a terminal opened inside the dir:-
      ```
      chmod +x Bob.sh DataAudit Attack fileCompare
      ```

### Start Data Audit

Now, we are ready to start the periodic data-audit process. Make sure the audio system in "Auditor" machine is turned on.

- Open two terminals: one inside the "Auditor" (Alice), and one inside the "Auditee" (Bob)
- run `./Bob.sh` inside "Auditee"
- run `./Alice.sh` inside "Auditor"

Observe the Auditor's terminal for some duration of time - verification must be successful in all instances. 

### Launch Attack

Now, we'll test the impact of attack. For this purpose we'll invoke our simulated attack software.

- Open another terminal inside the "Auditee" (Bob)
- to invoke the attacker on the data-file, execute the following command:-
    - `./Attack <filename> <attack_proportion> <blocksize>` 
    - For example :
      ```
        ./Attack 15MBData.csv  0.01 2000
    ```
    - where:-
        - the data-file name is '15MBData.csv'
        - blocksize = 2000 (bytes) and,
        - attack_proportion = 0.01

Attack should be immediately detected at the Auditor's terminal. 

To restore the original data-file run the following command:-

 `cp 15MBData2.csv 15MBData.csv`

## 🛠️ Observing Statistics

In each of the directories : "Auditor", "Auditee" and "CSP-Admin" - a `Statistics.txt` file will be generated. These files will report the  execution times of each instance of the following algorithms:-

- Setup Time
- Average Tag Generation Time (for one block) 
- Proof Generation Time (excluding Disk I/O and chellenge-vector generation time)
- Proof Verification Time (excluding Disk I/O and chellenge-vector generation time)

## 🔨 Verifying the Correctness of the Simulated Attack-tool

To check the correctness of our simulated Attack program, we have the `filecompare` program. To compare the difference between two files run the following command on a terminal inside the Auditee dir:-
```
./fileCompare <filename1> <filename2> <blocksize>
```


For example:-
```
./fileCompare 15MBData.csv 15MBData2.csv 2000
```
Note that file1 and file2 must be of the same size.

The above command will report the following:-
- Total number of blocks in the files
- Total no. of mismatching blocks between the two files
- Proportion of mismatching blocks 
- List of all mismatching block-indices

## 🚧 The Source Codes

Although our repository is pre-compiled, you may still compile the source codes to regenerate the executable files: `DataAudit`, `Attack` and `fileCompare`, specially if you wish to make any modifications to the source codes. All the souce codes are kept inside the "Source Codes" directory. To Compile the codes, cd inside the directory and run the following:-
```
make clean
make
```
## 🧩 Experimentation

In order to perform experimentataion with our developped software we have also provided a set of SHELL scripts kept inside the "Experimentation Scripts" directory. You may use these scripts to measure the accuracy of attack detection or, the average execution time of the different algorithms for different protocol configuration parameters or, for different attack intensities or different file-sizes etc.

## 🙌 Credits:-

[🔝 Back to Top](#)

- JUNAID ALAM
- DR. SOUMYADEV MAITY 

This work is part of the Project titled "Reliable and Privacy Preserving Data Auditing for FOG assisted Cyber Physical Systems (FOG-CPS)" funded by C3I-Hub, IIT Kanpur, Proposal Number: 1045, Sanction Order No.: IHUB-NTIHAC/2021/01/3.

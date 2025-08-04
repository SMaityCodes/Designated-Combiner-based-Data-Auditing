# Designated Combiner based Data Auditing

## 📁 Project Structure

```
Designated Combiner based Data Auditing
├── Admin-KGC-CSP/              
│   ├── makefile           # make file for Admin-KGC-CPS protocol
|   ├── a.param            # PBC parameters
│   ├── DataAudit          # [executable] the main data-auditing software
│   └── in.ods             # A sample data-file
│
├── Auditee/              
│   ├── Attack              # [executable] the attack-software
│   ├── Bob.sh              # SHELL script for auditee's protocol
|   ├── a.param             # PBC parameters
│   ├── DataAudit           # [executable] the main data-auditing software
|   ├── fileCompare         # [executable] software for comparing the difference between two data-files
|   ├── in.ods              # A sample data-file
│   └── in2.ods             # Copy of the sample data-file
│
├── Auditor/              
│   ├── Alice.sh            # SHELL script for auditor's protocol
│   ├── Failure_Msg.ogg     # supporting audio file 
│   ├── Success_Msg.ogg     # supporting audio file
|   ├── a.param             # PBC parameters
│   └── DataAudit           # [executable] the main data-auditing software
│
└── Source Codes/            # All source codes
    ├── dataAudit.c
    ├── audit_utils.h
    ├── file_utils.h
    ├── pbc_utils.h
    ├── Attack.cpp
    ├── fileCompare.cpp
    ├── in.ods
    ├── a.param
    ├── a1.param    
    └── Makefile

```
**Note**: the copies of the `a.param` file in all four directories - "Admin-KGC-CSP", "Auditee", "Auditor" and "Source Codes" while `in.ods` file in all three directories - "Admin-KGC-CSP", "Auditee" and "Source Codes" must be consistent.

## ⚙️ Prerequisite:-

- UNIX OS (Ubuntu)
- GNU Compiler Collection (GCC)
- PBC Library

In order to install PBC from [here](https://github.com/wellsaid/pbc-0.5.14): A Pairing Based Cryptography Libaray
```
sudo apt-cache pkgnames | grep -i crypto++
sudo apt-get install libcrypto++-utils
sudo apt-get install libcrypto++8
sudo apt-get install libcrypto++-dev
sudo apt-get install libcrypto++-doc
```

## 🚀 How to Use This Repository?
You can test it in three different systems connected by a network - one system working as the Admin-KGC-CSP, one acting as the auditor and the other one acting as the auditee. However, you can also test it on a single system (in different terminals). Our files are written assuming the testing to be done on same system. Make the changes accordingly if you test on different systems.

- clone the repository
- copy the "Admin-KGC-CSP" directory into the Admin-KGC-CSP system
- copy the "Auditor" directory into the auditor system
- copy the "Auditee" directory into the auditee system

### Configure the Auditor, Auditee and the Admin-KGC-CSP

- Go inside "Admin-KGC-CSP" dir:-
    - make sure that the path and filename of the data-file (file to be audited) is properly mentioned in the  `makefile`
    - run the following command on a terminal opened inside the dir:-
      ```
      chmod +x DataAudit
      make
      ```
    - the above will generate the admin's master-secret-key *MSK.bin*, the public parameters *localParams.bin* and the *metadata* of the data-file using *sigma.bin*
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

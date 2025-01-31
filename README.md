# PASTA, TransCov, ValComb and the Random Approach

**Paper Title**: Different Approaches for Testing Body Sensor Network Applications

**Submitted to**: Journal of Systems and Software (JSS)

**Abstract**: Body Sensor Networks (BSNs) have emerged from a recent thrust on wearable biosensor technology development. They provide an inexpensive means to continuously monitor the health status of patients and timely detect potential risks. Notwithstanding the growing interest attracted by BSNs and their critical nature, in the literature, there is a lack of testing approaches for BSNs. In this work, we fill this gap and propose a generic approach, called GATE4BSN , consisting of the abstract steps to be taken for testing the BSN. GATE4BSN is then instantiated in three approaches, namely PASTA (PAtient Simulation for Testing of bsn Applications), ValComb (Sensor Values Combination), and TransCov (Sensor Transitions Coverage). PASTA simulates patients by considering a set of sensors and by mimicking the trend of each sensor via a Discrete Time Markov Chain (DTMC). ValComb makes use of all possible combinations of sensor risk levels to explore different health conditions of a patient. Finally, TransCov employs a dummy patient in which all the transitions in the DTMCs are labeled with equal probability, and DTMCs are executed until all the transitions are covered. We empirically assessed through experiments our three approaches in terms of their effectiveness, i.e., the ability to detect failures, and compared them with a baseline approach. PASTA, ValComb, and TransCov uncovered unknown failures in the system under test, an open source BSN, and showed better effectiveness when compared to a random baseline. Also, statistical analysis shows that while PASTA is the most effective approach, ValComb is the closest to PASTA in terms of effectiveness but approximately 82 times faster.
  
## **Configure the SA-BSN**:
1. Download and install the [ROS Noetic for Ubuntu 20.04](http://wiki.ros.org/noetic/Installation/Ubuntu)
2. Download and install the [SA-BSN](https://github.com/lesunb/bsn/tree/1c45cd8f4c43e36fcf5665940d5ce7c66b907b31)

## **Clone this repository**:
[comment]: <> ($ git clone https://github.com/fedebotu/clone-anonymous-github.git && cd clone-anonymous-github)
[comment]: <> ($ python3 src/download.py --url https://anonymous.4open.science/r/PASTA_SAFECOMP-5F11/)
[comment]: <> ($ cd PASTA_SAFECOMP-5F11/)

```
$ git clone https://github.com/samirasilva/Paper_JSS.git
$ cd Paper_JSS
```
## 1. PASTA
### **Preparing the SA-BSN to PASTA**:
1. Replace the *G4T1.cpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/G4T1.cpp bsn_path/src/sa-bsn/target_system/components/component/src/g4t1/G4T1.cpp
```
2. Replace the *DataGenerator.cpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/DataGenerator.cpp bsn_path/src/sa-bsn/src/libbsn/src/generator/DataGenerator.cpp
```
3. Replace the *DataGenerator.hpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/DataGenerator.hpp bsn_path/src/sa-bsn/src/libbsn/include/libbsn/generator/DataGenerator.hpp
``` 
4. Replace the *PatientModule.cpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/PatientModule.cpp bsn_path/src/sa-bsn/environment/patient/src/PatientModule.cpp
```
5. Replace the *PatientModule.hpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/PatientModule.hpp bsn_path/src/sa-bsn/environment/patient/include/PatientModule.hpp
``` 
6. Replace the *g4t1.launch* file:
```
$ \cp PASTA/BSN_Files_to_Replace/g4t1.launch bsn_path/src/sa-bsn/configurations/target_system/g4t1.launch
```
7. Replace the *CentralHub.cpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/CentralHub.cpp bsn_path/src/sa-bsn/target_system/components/component/src/CentralHub.cpp
```
8. Replace the *CentralHub.hpp* file:
```
$ \cp PASTA/BSN_Files_to_Replace/CentralHub.hpp bsn_path/src/sa-bsn/target_system/components/component/include/component/CentralHub.hpp
```
9. Replace the *run.sh* file:
```
$ \cp PASTA/BSN_Files_to_Replace/run.sh bsn_path/run.sh
```
10. Build the SA-BSN:
```
$ (cd bsn_path ; catkin_make)
```
<sub>(Replace "bsn_path" with the path to the BSN folder.)</sub>

### **Running PASTA**:

#### **1. Generation of T-way Combinations**:
*(Skip this step if you want to use the [provided t-way combinations](https://github.com/samirasilva/Paper_JSS/blob/main/PASTA/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt))*

1. Download the [ACTS 3.2 tool](https://github.com/usnistgov/combinatorial-testing-tools).
2. Execute the ACTS tool (double-click on the *jar* file).
3. Click on *System->Open...* and choose the file `Paper_JSS/PASTA/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt` previously downloaded on your computer.
4. Click on *Edit->Modify...* to modify "parameters" or "constraints" for the combinations.
5. Click on *Build->Build* to regenerate the t-way combinations.
6. Finally, save your modifications with *System->Save*.

#### **2. Converting T-way Combinations into Patients**:
*(Step needed only if the previous one has been performed)*

1. Execute the script `conver_tm_into_patient.py` to transform each t-way combination into a patient:
```
$ python PASTA/PASTA_Scripts/conver_tm_into_patient.py PASTA/Output_Files/2_Test_Patients PASTA/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt

```
`PASTA/Output_Files/2_Test_Patients` is the folder where you want to save the patients.

#### **3. Executing PASTA**:
1. Open the terminal and type:
```
bash PASTA/PASTA_Scripts/script_pasta_jss.sh PASTA/Output_Files/2_Test_Patients 'number_of_patients' 'execution_time' 'number_of_executions'
```
Note: 
Replace `number_of_patients` with the number of patient files in `PASTA/Output_Files/2_Test_Patients` (e.g., 278 patients).
`execution_time` determines how much time each patient should be run (e.g., 30 seconds).
`number_of_executions` is the amount of times that PASTA should be run (e.g, 25 times).

2. To parse the log files, collect Sensor Readings and BSN Outcomes, and compute the Expected Outcome, type:
```
python  PASTA/PASTA_Scripts/read_all_output.py 'number_of_patients' 'number_of_executions'

```

#### **Outcome Files**:
- [1 - ACTS Test Set](https://github.com/samirasilva/Paper_JSS/blob/main/PASTA/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt) (Configuration of the ACTS tool and the output (t-way combinations))
- [2 - Test Patients](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/2_Test_Patients) (Patient Configuration Files for the BSN)
- [3 - BSN Logs ](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/3_BSN_Logs) (Log files generated by the SA-BSN)
- [4 - Sensor Readings, BSN Outcomes and Expected Outcomes](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/4_Sensor_Readings_BSN_Outcome_and_Expected_Outcome) (Parsing of BSN Logs)
- [5 - Summary of Results](https://github.com/samirasilva/Paper_JSS/blob/main/PASTA/Output_Files/5_Summary_Of_Results/Pasta_experiments.ods) (Testing Results)

  
## 2. TransCov
### **Preparing the SA-BSN to TransCov**:
1. Replace the *G4T1.cpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/G4T1.cpp bsn_path/src/sa-bsn/target_system/components/component/src/g4t1/G4T1.cpp
```
2. Replace the *DataGenerator.cpp* file:T
```
$ \cp TransCov/BSN_Files_to_Replace/DataGenerator.cpp bsn_path/src/sa-bsn/src/libbsn/src/generator/DataGenerator.cpp
```
3. Replace the *DataGenerator.hpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/DataGenerator.hpp bsn_path/src/sa-bsn/src/libbsn/include/libbsn/generator/DataGenerator.hpp
``` 
4. Replace the *PatientModule.cpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/PatientModule.cpp bsn_path/src/sa-bsn/environment/patient/src/PatientModule.cpp
```
5. Replace the *PatientModule.hpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/PatientModule.hpp bsn_path/src/sa-bsn/environment/patient/include/PatientModule.hpp
``` 
6. Replace the *g4t1.launch* file:
```
$ \cp TransCov/BSN_Files_to_Replace/g4t1.launch bsn_path/src/sa-bsn/configurations/target_system/g4t1.launch
```
7. Replace the *CentralHub.cpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/CentralHub.cpp bsn_path/src/sa-bsn/target_system/components/component/src/CentralHub.cpp
```
8. Replace the *CentralHub.hpp* file:
```
$ \cp TransCov/BSN_Files_to_Replace/CentralHub.hpp bsn_path/src/sa-bsn/target_system/components/component/include/component/CentralHub.hpp
```
9. Replace the *run.sh* file:
```
$ \cp TransCov/BSN_Files_to_Replace/run.sh bsn_path/run.sh
```
10. Build the SA-BSN:
```
$ (cd bsn_path ; catkin_make)
```
<sub>(Replace "bsn_path" with the path to the BSN folder.)</sub>

### **Running TransCov**:

#### **1. Generation of T-way Combinations**:
*(Skip this step if you want to use the [provided t-way combinations](https://github.com/samirasilva/Paper_JSS/blob/main/TransCov/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt))*

1. Download the [ACTS 3.2 tool](https://github.com/usnistgov/combinatorial-testing-tools).
2. Execute the ACTS tool (double-click on the *jar* file).
3. Click on *System->Open...* and choose the file `Paper_JSS/TransCov/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt` previously downloaded on your computer.
4. Click on *Edit->Modify...* to modify "parameters" or "constraints" for the combinations.
5. Click on *Build->Build* to regenerate the t-way combinations.
6. Finally, save your modifications with *System->Save*.

#### **2. Converting T-way Combinations into Patients**:
*(Step needed only if the previous one has been performed)*

1. Execute the script `conver_tm_into_patient.py` to transform each t-way combination into a patient:
```
$ python TransCov/TransCov_Scripts/conver_tm_into_patient.py TransCov/Output_Files/2_Test_Patients TransCov/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt

```
`TransCov/Output_Files/2_Test_Patients` is the folder where you want to save the patients.

#### **3. Executing TransCov**:
1. Open the terminal and type:
```
bash TransCov/TransCov_Scripts/script_pasta_jss.sh TransCov/Output_Files/2_Test_Patients 'number_of_patients' 'execution_time' 'number_of_executions'
```
Note: 
Replace `number_of_patients` with the number of patient files in `PASTA/Output_Files/2_Test_Patients` (e.g., 278 patients).
`execution_time` determines how much time each patient should be run (e.g., 30 seconds).
`number_of_executions` is the amount of times that PASTA should be run (e.g, 25 times).

2. To parse the log files, collect Sensor Readings and BSN Outcomes, and compute the Expected Outcome, type:
```
python  TransCov/TransCov_Scripts/read_all_output.py 'number_of_patients' 'number_of_executions'

```

#### **Outcome Files**:
- [1 - ACTS Test Set](https://github.com/samirasilva/Paper_JSS/blob/main/PASTA/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt) (Configuration of the ACTS tool and the output (t-way combinations))
- [2 - Test Patients](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/2_Test_Patients) (Patient Configuration Files for the BSN)
- [3 - BSN Logs ](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/3_BSN_Logs) (Log files generated by the SA-BSN)
- [4 - Sensor Readings, BSN Outcomes and Expected Outcomes](https://github.com/samirasilva/Paper_JSS/tree/main/PASTA/Output_Files/4_Sensor_Readings_BSN_Outcome_and_Expected_Outcome) (Parsing of BSN Logs)
- [5 - Summary of Results](https://github.com/samirasilva/Paper_JSS/blob/main/PASTA/Output_Files/5_Summary_Of_Results/Pasta_experiments.ods) (Testing Results)

## 3. ValComb
### **Preparing the SA-BSN to ValComb**:
1. Replace the *G4T1.cpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/G4T1.cpp bsn_path/src/sa-bsn/target_system/components/component/src/g4t1/G4T1.cpp
```
2. Replace the *G4T1.hpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/G4T1.hpp bsn_path/src/sa-bsn/target_system/components/component/include/component/g4t1/G4T1.hpp
```
3. Replace the *DataGenerator.cpp* file:T
```
$ \cp ValComb/BSN_Files_to_Replace/DataGenerator.cpp bsn_path/src/sa-bsn/src/libbsn/src/generator/DataGenerator.cpp
```
4. Replace the *DataGenerator.hpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/DataGenerator.hpp bsn_path/src/sa-bsn/src/libbsn/include/libbsn/generator/DataGenerator.hpp
``` 
5. Replace the *PatientModule.cpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/PatientModule.cpp bsn_path/src/sa-bsn/environment/patient/src/PatientModule.cpp
```
6. Replace the *PatientModule.hpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/PatientModule.hpp bsn_path/src/sa-bsn/environment/patient/include/PatientModule.hpp
``` 
7. Replace the *g4t1.launch* file:
```
$ \cp ValComb/BSN_Files_to_Replace/g4t1.launch bsn_path/src/sa-bsn/configurations/target_system/g4t1.launch
```
8. Replace the *CentralHub.cpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/CentralHub.cpp bsn_path/src/sa-bsn/target_system/components/component/src/CentralHub.cpp
```
9. Replace the *CentralHub.hpp* file:
```
$ \cp ValComb/BSN_Files_to_Replace/CentralHub.hpp bsn_path/src/sa-bsn/target_system/components/component/include/component/CentralHub.hpp
```
10. Replace the *run.sh* file:
```
$ \cp ValComb/BSN_Files_to_Replace/run.sh bsn_path/run.sh
```
11. Build the SA-BSN:
```
$ (cd bsn_path ; catkin_make)
```
<sub>(Replace "bsn_path" with the path to the BSN folder.)</sub>

### **Running ValComb**:

#### **1. Generation of T-way Combinations**:
*(Skip this step if you want to use the [provided t-way combinations](https://github.com/samirasilva/Paper_JSS/blob/main/ValComb/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt))*

1. Download the [ACTS 3.2 tool](https://github.com/usnistgov/combinatorial-testing-tools).
2. Execute the ACTS tool (double-click on the *jar* file).
3. Click on *System->Open...* and choose the file `Paper_JSS/ValComb/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt` previously downloaded on your computer.
4. Click on *Edit->Modify...* to modify "parameters" or "constraints" for the combinations.
5. Click on *Build->Build* to regenerate the t-way combinations.
6. Finally, save your modifications with *System->Save*.

#### **2. Converting T-way Combinations into Patients**:
*(Step needed only if the previous one has been performed)*

1. Execute the script `conver_tm_into_patient.py` to transform each t-way combination into a patient:
```
$ python ValComb/ValComb_Scripts/conver_tm_into_patient.py ValComb/Output_Files/2_Test_Patients ValComb/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt

```
`ValComb/Output_Files/2_Test_Patients` is the folder where you want to save the patients.

#### **3. Executing ValComb**:
1. Open the terminal and type:
```
bash ValComb/ValComb_Scripts/script_valcomb_jss.sh ValComb/Output_Files/2_Test_Patients 'number_of_patients' 'execution_time' 'number_of_executions'
```
Note: 
Replace `number_of_patients` with the number of patient files in `ValComb/Output_Files/2_Test_Patients` (e.g., 278 patients).
`execution_time` determines how much time each patient should be run (e.g., 30 seconds).
`number_of_executions` is the amount of times that ValComb should be run (e.g, 25 times).

2. To parse the log files, collect Sensor Readings and BSN Outcomes, and compute the Expected Outcome, type:
```
python ValComb/ValComb_Scripts/read_all_output.py 'number_of_patients' 'number_of_executions'

```

#### **Outcome Files**:
- [1 - ACTS Test Set](https://github.com/samirasilva/Paper_JSS/blob/main/ValComb/Output_Files/1_ACTS_BSN_Test_Set_t2/1_ACTS_BSN_Test_Set_t_2.txt) (Configuration of the ACTS tool and the output (t-way combinations))
- [2 - Test Patients](https://github.com/samirasilva/Paper_JSS/tree/main/ValComb/Output_Files/2_Test_Patients) (Patient Configuration Files for the BSN)
- [3 - BSN Logs ](https://github.com/samirasilva/Paper_JSS/tree/main/ValComb/Output_Files/3_BSN_Logs) (Log files generated by the SA-BSN)
- [4 - Sensor Readings, BSN Outcomes and Expected Outcomes](https://github.com/samirasilva/Paper_JSS/tree/main/ValComb/Output_Files/4_Sensor_Readings_BSN_Outcome_and_Expected_Outcome) (Parsing of BSN Logs)
- [5 - Summary of Results](https://github.com/samirasilva/Paper_JSS/blob/main/ValComb/Output_Files/5_Summary_Of_Results/Pasta_experiments.ods) (Testing Results)
  
## 4. Random
### **Preparing the SA-BSN to Random**:
1. Replace the *G4T1.cpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/G4T1.cpp bsn_path/src/sa-bsn/target_system/components/component/src/g4t1/G4T1.cpp
```
2. Replace the *DataGenerator.cpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/DataGenerator.cpp bsn_path/src/sa-bsn/src/libbsn/src/generator/DataGenerator.cpp
```
3. Replace the *DataGenerator.hpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/DataGenerator.hpp bsn_path/src/sa-bsn/src/libbsn/include/libbsn/generator/DataGenerator.hpp
``` 
4. Replace the *PatientModule.cpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/PatientModule.cpp bsn_path/src/sa-bsn/environment/patient/src/PatientModule.cpp
``` 
5. Replace the *g4t1.launch* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/g4t1.launch bsn_path/src/sa-bsn/configurations/target_system/g4t1.launch
```
6. Replace the *CentralHub.cpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/CentralHub.cpp bsn_path/src/sa-bsn/target_system/components/component/src/CentralHub.cpp
```
7. Replace the *CentralHub.hpp* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/CentralHub.hpp bsn_path/src/sa-bsn/target_system/components/component/include/component/CentralHub.hpp
```
8. Replace the *run.sh* file:
```
$ \cp Random_Approach/BSN_Files_to_Replace/run.sh bsn_path/run.sh
```
9. Build the SA-BSN:
```
$ (cd bsn_path ; catkin_make)
```
<sub>(Replace "bsn_path" with the path to the BSN folder.)</sub>

### **Running Random**:
1. Unzip the Random Files:
```
$ (cd Random_Approach/Output_Files/2_BSN_Logs ; for f in *.tar.xz; do tar -xvf "$f"; done)
$ (cd Random_Approach/Output_Files/3_Sensor_Readings_BSN_Outcome_and_Expected_Outcome ; for f in *.tar.xz; do tar -xvf "$f"; done)
```
2. Type:
```
bash Random_Approach/Random_Scripts/script_baseline_jss.sh Random_Approach/Output_Files/1_Test_Patients 1 'execution_time' 'number_of_executions'
```
To compare Random to PASTA, the `execution_time` of Random should be equal to the number of patients in PASTA x execution time in PASTA (e.g., 278x30=8340 seconds).
`number_of_executions` is the number of times that Random should be run (e.g., 10 times).

3. To parse the log files, collect Sensor Readings and BSN Outcomes, and compute the Expected Outcome, type:
```
python Random_Approach/Random_Scripts/read_all_output.py 1 'number_of_executions'
```

### **Outcome Files**:
- [1 - Test Patient](https://github.com/samirasilva/Paper_JSS/blob/main/Random_Approach/Output_Files/1_Test_Patients/patient_0.launch) (Patient Configuration File for the BSN)
- [2 - BSN Logs ](https://github.com/samirasilva/Paper_JSS/blob/main/Random_Approach/Output_Files/2_BSN_Logs/) (Log files generated by the SA-BSN)
- [3 - Sensor Readings, BSN Outcomes and Expected Outcomes](https://github.com/samirasilva/Paper_JSS/blob/main/Random_Approach/Output_Files/3_Sensor_Readings_BSN_Outcome_and_Expected_Outcome/ParseLog.tar.xz) (Parsing of BSN Logs)
- [4 - Summary of Results](https://github.com/samirasilva/Paper_JSS/blob/main/Random_Approach/Output_Files/4_Summary_Of_Results/)(Testing Results)

## **Statistical Tests**
- [1 - Datasets](https://github.com/samirasilva/Paper_JSS/blob/main/Scripts_Statistical_Tests/dataset.tar.xz) (Dataset with Test Cases (Sensor Readings and Expected Outcomes) and BSN Outcomes)
- [2 - Script ](https://github.com/samirasilva/Paper_JSS/blob/main/Scripts_Statistical_Tests/statistical_analysis.R) (Script to compute statistical tests)

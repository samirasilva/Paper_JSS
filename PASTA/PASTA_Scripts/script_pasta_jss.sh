#!/bin/bash


#Example of call: 
#bash script_coverage.sh 'Path_to_Patients_folder' 'number_of_patients' 'execution_time' 'number_of_executions'
#or
#bash script_coverage.sh

if [[ "$#" -gt 4 ]]; then
    echo "Too many arguments were passed!"
    exit 1
fi

if [[ "$#" -eq 0 ]]; then
    original_patient_folder="./patient"
    number_of_patients=278
    execution_time=30 
    number_of_executions=10
else
    original_patient_folder=$1
    number_of_patients=$2
    execution_time=$3
    number_of_executions=$4
fi

directory_ros="${HOME}/.ros/"
bsn_patient_folder="${HOME}/sa-bsn/src/sa-bsn/configurations/environment/"
rm -f "${HOME}/sa-bsn/Cov_all_patients.txt"
rm -f "${HOME}/sa-bsn/Cov_all_patients_old.txt"

for ((j=0;j<"$number_of_executions";j++));do
   #Create the output folder
   directory_Output="./output_${j}/"
   if [ ! -d "$directory_Output" ]; then
       mkdir -p "$directory_Output"
   fi

   #Copy the patients to the BSN folder
   if [ -z "$(ls -A ${bsn_patient_folder})" ]; then
      echo "Empty"
   else
      rm "${bsn_patient_folder}"*".launch"
   fi
   cp -a "${original_patient_folder}/." "${bsn_patient_folder}"

   #Execute each patient and copy the resulting log file to the output folder
   origin=$(pwd)
   #for ((i=0;i<"$number_of_patients";i++));do
   for ((i=0;i<278;i++));do
      cd "${HOME}/sa-bsn"
      source devel/setup.bash
      start=`date +%s`
      bash run.sh $execution_time $i
      end=`date +%s`
      temp="${directory_Output}tempo.txt"
      cd "$origin"
      echo `expr $end - $start` seconds. >> "$temp" 
      destiny="${directory_Output}g4t1_${i}-1-stdout.log"
      cp "${directory_ros}log/latest/g4t1-1-stdout.log" "$destiny"
      destiny_cov="${directory_Output}cov_${i}.txt"
      #minimumsize=10
      #actualsize=$(wc -c <"${HOME}/sa-bsn/Cov_all_patients.txt")
      
      if test "$(wc -l "${HOME}/sa-bsn/Cov_all_patients.txt" | cut -f1 -d' ')" -gt 6; then
        echo "Greater than zero!"
        cp "${HOME}/sa-bsn/Cov_all_patients.txt" "$destiny_cov"
      	rm "${HOME}/sa-bsn/Cov_all_patients.txt"
      	rm "${HOME}/sa-bsn/Cov_all_patients_old.txt"
      else
        cp "${HOME}/sa-bsn/Cov_all_patients_old.txt" "$destiny_cov"
      	rm "${HOME}/sa-bsn/Cov_all_patients.txt"
      	rm "${HOME}/sa-bsn/Cov_all_patients_old.txt"    
      fi
      
      #if [ $actualsize -ge $minimumsize ]; then
      #  echo "Greater than zero!"
      #  cp "${HOME}/sa-bsn/Cov_all_patients.txt" "$destiny_cov"
      #  rm "${HOME}/sa-bsn/Cov_all_patients.txt"
      #fi
   done
done

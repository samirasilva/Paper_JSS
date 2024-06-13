#!/bin/bash


#Example of call: 
#bash read_all_output.sh 'number_of_patients' 'number_of_executions'
#or
#bash read_all_output.sh

if [[ "$#" -gt 2 ]]; then
    echo "Too many arguments were passed!"
    exit 1
fi

if [[ "$#" -eq 0 ]]; then
    number_of_patients=278
    number_of_executions=10
else
    number_of_patients=$1
    number_of_executions=$2
fi


for ((j=0;j<"$number_of_executions";j++));do
       python read_output_prob_t2.py "./output_${j}" 'output_sensor_readings.xlsx' "$number_of_patients"
done

python compute_average_cov.py

python compute_total_time.py


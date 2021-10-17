#!bin/bash

unzip LKH-3.0.6.zip
cp execute-lkh-CC-2step.sh LKH-3.0.6/

echo "Starting"

for (( instance_id=0; instance_id <= 9999; instance_id++));
do
    touch params_WK_${instance_id}.txt
    echo "PROBLEM_FILE = ../../../Instances/CVRP/WK_test_${instance_id}.vrp" >> params_WK_${instance_id}.txt
    echo "SPECIAL" >> params_WK_${instance_id}.txt
    echo "MAX_TRIALS = 100000" >> params_WK_${instance_id}.txt
    echo "SEED = 1" >> params_WK_${instance_id}.txt
    echo "RUNS = 1" >> params_WK_${instance_id}.txt
    echo "TIME_LIMIT = 5" >> params_WK_${instance_id}.txt
    echo "OUTPUT_TOUR_FILE = OUTPUT_TOUR_FILE/WK_test_${instance_id}.txt" >> params_WK_${instance_id}.txt
    echo "TOUR_FILE = TOUR_FILE/WK_test_${instance_id}.txt" >> params_WK_${instance_id}.txt
    echo "EOF" >> params_WK_${instance_id}.txt
    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt
    rm params_WK_${instance_id}.txt
done

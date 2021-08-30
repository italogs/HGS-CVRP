#!/bin/bash

cd LKH-3.0.6/

for (( instance_id=0; instance_id <= 9999; instance_id++));
do
    rm params.txt
    echo "PROBLEM_FILE = ../../../Instances/CVRP/WK_test_${instance_id}.vrp" > params.txt
    echo "SPECIAL" >> params.txt
    echo "MAX_TRIALS = 10000" >> params.txt
    echo "TRACE_LEVEL = 1" >> params.txt
    echo "SEED = 1234" >> params.txt
    echo "RUNS = 1" >> params.txt
    echo "TIME_LIMIT = 5" >> params.txt
    echo "OUTPUT_TOUR_FILE = WK_test_${instance_id}.txt" >> params.txt
    echo "TOUR_FILE = WK_test_${instance_id}.txt" >> params.txt
    echo "EOF" >> params.txt
    ./LKH params.txt
done

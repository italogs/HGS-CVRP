#!/bin/bash
#SBATCH --time=0:01:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=4G

# 1
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait

# 2
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait


# 3
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait

# 4
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait

# 5
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait

# 6
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait

# 7
for (( it =0 ; it < 5;  it++));
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

    ./LKH params_WK_${instance_id}.txt &> output_LKH/stdout_WK_test_${instance_id}.txt &
    
    instance_id=$(($instance_id + 1))
done
wait


#!/bin/bash

make clean && make
mkdir -p Solutions/
mkdir -p Solutions-DPDP/
mkdir -p outputs/
time=5
seed=1
    
for (( instance_id=0; instance_id <= 9999; instance_id++));
do

    useDPDP=2
    crossover=1
    qsub -V -v ARGS="./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed}",OUTPUT="Solutions/output_WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt" run_experiments.pbs

    useDPDP=2
    crossover=9
    qsub -V -v ARGS="./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed}",OUTPUT="Solutions/output_WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt" run_experiments.pbs

    useDPDP=4
    crossover=1
    qsub -V -v ARGS="./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed}",OUTPUT="Solutions/output_WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt" run_experiments.pbs

    useDPDP=4
    crossover=9    
    qsub -V -v ARGS="./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed}",OUTPUT="Solutions/output_WK_test_${instance_id}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt" run_experiments.pbs

done
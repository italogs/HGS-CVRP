#!/bin/bash
#SBATCH --time=00:20:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=3
#SBATCH --mem=20G

seed=1
time=5
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    
    
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &


    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &

    wait

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &


    useDPDP=0
    crossover=10
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &

    useDPDP=1
    crossover=10
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &

    wait
done

wait

#!/bin/bash
#SBATCH --time=00:10:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G


for (( it = ${instance_id} ; it <= ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    for (( seed=1; seed <= 1; seed++));
    do
        ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    done
    # wait

    useDPDP=1
    crossover=1
    for (( seed=1; seed <= 1; seed++));
    do
        ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    done
    # wait

    useDPDP=0
    crossover=9
    for (( seed=1; seed <= 1; seed++));
    do
        ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    done
    # wait

    useDPDP=1
    crossover=9
    for (( seed=1; seed <= 1; seed++));
    do
        ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    done
    # wait

    wait


done


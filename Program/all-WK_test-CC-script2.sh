#!/bin/bash
#SBATCH --time=00:10:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G

for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    seed=1
    time=5
    
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &

    if ((it % 2 == 1));
    then
        
        wait
    fi

done

wait

#!/bin/bash
#SBATCH --time=00:01:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G


useDPDP=0
crossover=1
for (( seed=1; seed <= 1; seed++));
do
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
done

useDPDP=1
crossover=1
for (( seed=1; seed <= 1; seed++));
do
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
done

wait

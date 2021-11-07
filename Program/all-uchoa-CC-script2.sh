#!/bin/bash
#SBATCH --time=00:01:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=20G


seed=1
useDPDP=0
crossover=1
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &


useDPDP=1
crossover=1
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    
wait

useDPDP=0
crossover=9
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    


useDPDP=1
crossover=9
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    


wait

useDPDP=0
crossover=10
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    


useDPDP=1
crossover=10
mkdir -p Solutions/useDPDP${useDPDP}_crossover${crossover}/
mkdir -p outputs/useDPDP${useDPDP}_crossover${crossover}/

./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}.txt &
    


wait

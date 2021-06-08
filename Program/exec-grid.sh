#!/bin/bash
#SBATCH --time=00:45:00
#SBATCH --account=ctb-crainct-ab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=40
#SBATCH --mem=400G

crossover=5
for (( seed=1; seed <= 5; seed++));
do
	./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_seed${seed}.sol -seed ${seed} -crossover ${crossover} -t ${time} >& Solutions/output_${instance}_crossover${crossover}_seed${seed}.txt &
	sleep 10
done

crossover=6
for (( seed=1; seed <= 5; seed++));
do
	./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_seed${seed}.sol -seed ${seed} -crossover ${crossover} -t ${time} >& Solutions/output_${instance}_crossover${crossover}_seed${seed}.txt &
	sleep 10
done

crossover=7
for (( seed=1; seed <= 5; seed++));
do
	./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_seed${seed}.sol -seed ${seed} -crossover ${crossover} -t ${time} >& Solutions/output_${instance}_crossover${crossover}_seed${seed}.txt &
	sleep 10
done

crossover=8
for (( seed=1; seed <= 5; seed++));
do
	./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_crossover${crossover}_seed${seed}.sol -seed ${seed} -crossover ${crossover} -t ${time} >& Solutions/output_${instance}_crossover${crossover}_seed${seed}.txt &
	sleep 10
done
wait
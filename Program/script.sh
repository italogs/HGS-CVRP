#!/bin/bash


time=400
# make clean && make
make clean && make myOS=MacOSX

seed=1
instances=(X-n106-k14 X-n153-k22 X-n670-k130 X-n766-k71 X-n819-k171 X-n856-k95 X-n936-k151 X-n1001-k43)
instances=(X-n106-k14)
for (( index=${#instances[@]}-1; index >= 0; index--));
do
    for ((seed=1; seed <= 5; seed++));
    do
        crossover=4
        ./genvrp ../Instances/CVRP/${instances[$index]}.vrp testes/${instances[$index]}_crossover${crossover}_heap_optimized_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} #> testes/output_${instances[$index]}_crossover${crossover}_heap_optimized_seed${seed}_time${time}.txt &
        
    done;
    wait
done;
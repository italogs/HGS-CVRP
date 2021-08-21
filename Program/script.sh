#!/bin/bash

make clean && make
mkdir -p Solutions/
mkdir -p Solutions-DPDP/
mkdir -p outputs/

for ((id=0; id <= 9999; id++));
do
    if ((id % 4 == 0));
    then
        wait;
    fi

    if ((id % 64 == 0));
    then
        echo "Current ID: ${id}" 
    fi

    time=5
    seed=1

    useDPDP=2
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} -useDPDP ${useDPDP} #> outputs/output_WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.txt
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} -useDPDP ${useDPDP} #> outputs/output_WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.txt
    

    useDPDP=4
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.txt &  
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_crossover${crossover}_seed${seed}_time${time}.txt &
    
done;

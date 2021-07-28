#!/bin/bash

make clean && make
mkdir -p Solutions/
mkdir -p Solutions-DPDP/
mkdir -p outputs/

seed=1
time=1

useDPDP=1
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=3
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=5
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;




useDPDP=2
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=4
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=6
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=7
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

useDPDP=9
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;


useDPDP=8
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;


useDPDP=10
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;


useDPDP=11
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;


useDPDP=12
for ((id=0; id <= 9999; id++));
do
    if ((id % 21 == 20));
    then
        echo "waiting useDPDP ${useDPDP}" 
        wait
    fi
    if ((id % 1000 == 0));
    then
        echo "Current ID: ${id}" 
    fi
    ./genvrp ../Instances/CVRP/WK_test_${id}.vrp Solutions/WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.sol -seed $seed -t ${time} -useDPDP ${useDPDP} > outputs/output_WK_test_${id}_useDPDP${useDPDP}_seed${seed}_time${time}.txt &
done;

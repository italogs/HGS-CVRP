#!/bin/bash
#SBATCH --time=00:60:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G

seed=1

nbGranular=5
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=10
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=20
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done


nbGranular=30
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=50
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=100
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &


    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/WK_test_${it}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/WK_test_${it}.vrp_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_WK_test_${it}.vrp_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

wait
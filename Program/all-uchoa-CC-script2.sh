#!/bin/bash
#SBATCH --time=00:01:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G
seed=1


nbGranular=5

useDPDP=0
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=1
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=0
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    
useDPDP=1
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

wait

nbGranular=10

useDPDP=0
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=1
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=0
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    
useDPDP=1
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

wait


nbGranular=20

useDPDP=0
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=1
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=0
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    
useDPDP=1
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

wait


nbGranular=30

useDPDP=0
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=1
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=0
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    
useDPDP=1
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

wait


nbGranular=50

useDPDP=0
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=1
crossover=1
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

useDPDP=0
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    
useDPDP=1
crossover=9
./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_crossover${crossover}_useDPDP${useDPDP}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

wait

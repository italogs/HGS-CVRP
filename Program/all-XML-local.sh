#!/bin/bash

make clean && make
mkdir -p Solutions/
mkdir -p Solutions-DPDP/
mkdir -p outputs/

trained_models=(10dim-5layers-500epochs) #10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
time=5
seed=1

for model in "${trained_models[@]}"
do
    echo $model

    instance="XML100_1111_12"
    


    for ((instance_id=0; instance_id < 1; instance_id++));
    do

        echo $instance_id
        
        nbGranular=20
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/teste.sol -useHeatmapLS 0 -useHeatmapOX 0 -seed ${seed} -t ${time} -nbGranular ${nbGranular} 

        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/teste.sol -useHeatmapLS 0 -useHeatmapOX 1 -seed ${seed} -t ${time} -nbGranular ${nbGranular} 

        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/teste.sol -useHeatmapLS 1 -useHeatmapOX 0 -seed ${seed} -t ${time} -nbGranular ${nbGranular} 


        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/teste.sol -useHeatmapLS 1 -useHeatmapOX 1 -seed ${seed} -t ${time} -nbGranular ${nbGranular} 



        # useHeatmap=0
        # crossover=9
        # nbGranular=20
        # ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_nbGranular${nbGranular}_time${time}_${model}.sol -crossover ${crossover} -seed ${seed} -t ${time} -useHeatmap ${useHeatmap} -nbGranular ${nbGranular} #&> outputs/output_WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_time${time}_nbGranular${nbGranular}_${model}.txt & 



        # useHeatmap=1
        # crossover=1
        # nbGranular=20
        # ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_nbGranular${nbGranular}_time${time}_${model}.sol -crossover ${crossover} -seed ${seed} -t ${time} -useHeatmap ${useHeatmap} -nbGranular ${nbGranular} #&> outputs/output_WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_time${time}_nbGranular${nbGranular}_${model}.txt & 


        # useHeatmap=1
        # crossover=9
        # nbGranular=20
        # ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_nbGranular${nbGranular}_time${time}_${model}.sol -crossover ${crossover} -seed ${seed} -t ${time} -useHeatmap ${useHeatmap} -nbGranular ${nbGranular} #&> outputs/output_WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_time${time}_nbGranular${nbGranular}_${model}.txt & 



        # nbGranular=30
        # ./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_nbGranular${nbGranular}_time${time}_${model}.sol -crossover ${crossover} -seed ${seed} -t ${time} -useHeatmap ${useHeatmap} -nbGranular ${nbGranular} &> outputs/output_WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_time${time}_nbGranular${nbGranular}_${model}.txt & 

        # nbGranular=50
        # ./genvrp ../Instances/CVRP/WK_test_${instance_id}.vrp Solutions/WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_nbGranular${nbGranular}_time${time}_${model}.sol -crossover ${crossover} -seed ${seed} -t ${time} -useHeatmap ${useHeatmap} -nbGranular ${nbGranular} # &> outputs/output_WK_test_${instance_id}_useHeatmap${useHeatmap}_crossover${crossover}_seed${seed}_time${time}_nbGranular${nbGranular}_${model}.txt & 

        wait
    done;

done ;

#!/bin/bash

# Creating folders
mkdir -p DPDP/Heatmaps_for_HGS/

trained_models=(10dim-5layers-1500epochs 10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
for model in "${trained_models[@]}"
do
    mkdir -p Solutions/useHeatmap0_crossover1/${model}/
    mkdir -p outputs/useHeatmap0_crossover1/${model}/

    mkdir -p Solutions/useHeatmap0_crossover9/${model}/
    mkdir -p outputs/useHeatmap0_crossover9/${model}/

    mkdir -p Solutions/useHeatmap1_crossover1/${model}/
    mkdir -p outputs/useHeatmap1_crossover1/${model}/

    mkdir -p Solutions/useHeatmap1_crossover9/${model}/
    mkdir -p outputs/useHeatmap1_crossover9/${model}/
done


# It will solve k instances at each sbatch
step_instances=5
time=5

for (( instance_id=0; instance_id <= 9999; ));
do
    sbatch --export=instance_id=${instance_id},time=${time},step_instances=${step_instances} all-WK_test-CC-script2.sh
    instance_id=$(($instance_id + $step_instances))
    break
done

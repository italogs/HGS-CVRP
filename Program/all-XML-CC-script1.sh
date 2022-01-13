#!/bin/bash

# Creating folders
mkdir -p DPDP/Heatmaps_for_HGS/

trained_models=(10dim-5layers-1500epochs) 
for model in "${trained_models[@]}"
do
    mkdir -p Solutions/${model}/useHeatmap0_crossover1/
    mkdir -p outputs/${model}/useHeatmap0_crossover1/

    mkdir -p Solutions/${model}/useHeatmap0_crossover9/
    mkdir -p outputs/${model}/useHeatmap0_crossover9/

    mkdir -p Solutions/${model}/useHeatmap1_crossover1/
    mkdir -p outputs/${model}/useHeatmap1_crossover1/

    mkdir -p Solutions/${model}/useHeatmap1_crossover9/
    mkdir -p outputs/${model}/useHeatmap1_crossover9/
done


# It will solve k instances at each sbatch
step_instances=500
time=5

for (( offset=0; offset <= 9999; ));
do
    sbatch --export=offset=${offset},time=${time},step_instances=${step_instances} all-XML-CC-script2.sh
    offset=$((${offset} + ${step_instances}))
done

#!/bin/bash

# Creating folders
mkdir -p DPDP/Heatmaps_for_HGS/

trained_models=( 10dim-5layers-500epochs) 
for model in "${trained_models[@]}"
do
    mkdir -p Solutions/5.2/${model}/useHeatmapLS0_crossoverType0/
    mkdir -p outputs/5.2/${model}/useHeatmapLS0_crossoverType0/


    mkdir -p Solutions/5.2/${model}/useHeatmapLS0_crossoverType2/
    mkdir -p outputs/5.2/${model}/useHeatmapLS0_crossoverType2/

    mkdir -p Solutions/5.2/${model}/useHeatmapLS1_crossoverType2/
    mkdir -p outputs/5.2/${model}/useHeatmapLS1_crossoverType2/


    mkdir -p Solutions/5.2/${model}/useHeatmapLS0_crossoverType1/
    mkdir -p outputs/5.2/${model}/useHeatmapLS0_crossoverType1/

    mkdir -p Solutions/5.2/${model}/useHeatmapLS1_crossoverType0/
    mkdir -p outputs/5.2/${model}/useHeatmapLS1_crossoverType0/

    mkdir -p Solutions/5.2/${model}/useHeatmapLS1_crossoverType1/
    mkdir -p outputs/5.2/${model}/useHeatmapLS1_crossoverType1/
done


# It will solve k instances at each sbatch
step_instances=100
time=5

for (( offset=0; offset <= 9999; ));
do
    sbatch --export=offset=${offset},time=${time},step_instances=${step_instances} all-XML-CC-script2-subsec5.2.sh
    offset=$((${offset} + ${step_instances}))
done

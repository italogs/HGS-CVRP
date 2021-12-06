#!/bin/bash

mkdir Solutions
mkdir Solutions-DPDP
mkdir outputs
mkdir -p DPDP/Heatmaps_for_HGS/

mkdir -p Solutions/useDPDP0_crossover1/
mkdir -p outputs/useDPDP0_crossover1/

mkdir -p Solutions/useDPDP0_crossover9/
mkdir -p outputs/useDPDP0_crossover9/

mkdir -p Solutions/useDPDP1_crossover1/
mkdir -p outputs/useDPDP1_crossover1/

mkdir -p Solutions/useDPDP1_crossover9/
mkdir -p outputs/useDPDP1_crossover9/



# We will execute 100 sbatchs commands. Each sbatch executes 20 processes in parallel (4 different crossover types on 5 different seeds)
for (( instance_id=0; instance_id <= 9999; ));
do
    sbatch --export=instance_id=${instance_id},time=5 all-WK_test-CC-script2.sh
    instance_id=$(($instance_id + 100))
done

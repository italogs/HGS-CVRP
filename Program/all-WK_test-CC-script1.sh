#!/bin/bash

mkdir Solutions
mkdir Solutions-DPDP
mkdir outputs

# We will execute 100 sbatchs commands. Each sbatch executes 20 processes in parallel (4 different crossover types on 5 different seeds)
for (( instance_id=0; instance_id <= 9999; ));
do
    sbatch --export=instance_id=${instance_id},time=5 all-WK_test-CC-script2.sh
    instance_id=$(($instance_id + 100))
done

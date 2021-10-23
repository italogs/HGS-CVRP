#!/bin/bash

mkdir CVRP/Solutions

for (( instance_id=0; instance_id <= 9999; ));
do
	sbatch --export=instance_id=${instance_id} $HOME/script-for-VRPSolver-CC-2step.sh
	instance_id=$(($instance_id + 5))
done

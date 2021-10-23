#!/bin/bash
#SBATCH --time=2:30:00
#SBATCH --account=def-vidalthi
#SBATCH --nodes=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20G

#1
$HOME/bap-singularity.sh /CVRP/src/run.jl /CVRP/data/WK_test/WK_test_${instance_id}.vrp -s /CVRP/input_sol/WK_test_${instance_id}.sol -o /CVRP/sol/WK_test_${instance_id}.sol &> $HOME/CVRP/Solutions/WK_test_${instance_id}.txt &


#2
instance_id=$(($instance_id + 1))
$HOME/bap-singularity.sh /CVRP/src/run.jl /CVRP/data/WK_test/WK_test_${instance_id}.vrp -s /CVRP/input_sol/WK_test_${instance_id}.sol -o /CVRP/sol/WK_test_${instance_id}.sol &> $HOME/CVRP/Solutions/WK_test_${instance_id}.txt &


#3
instance_id=$(($instance_id + 1))
$HOME/bap-singularity.sh /CVRP/src/run.jl /CVRP/data/WK_test/WK_test_${instance_id}.vrp -s /CVRP/input_sol/WK_test_${instance_id}.sol -o /CVRP/sol/WK_test_${instance_id}.sol &> $HOME/CVRP/Solutions/WK_test_${instance_id}.txt &


#4
instance_id=$(($instance_id + 1))
$HOME/bap-singularity.sh /CVRP/src/run.jl /CVRP/data/WK_test/WK_test_${instance_id}.vrp -s /CVRP/input_sol/WK_test_${instance_id}.sol -o /CVRP/sol/WK_test_${instance_id}.sol &> $HOME/CVRP/Solutions/WK_test_${instance_id}.txt &


#5
instance_id=$(($instance_id + 1))
$HOME/bap-singularity.sh /CVRP/src/run.jl /CVRP/data/WK_test/WK_test_${instance_id}.vrp -s /CVRP/input_sol/WK_test_${instance_id}.sol -o /CVRP/sol/WK_test_${instance_id}.sol &> $HOME/CVRP/Solutions/WK_test_${instance_id}.txt &

wait
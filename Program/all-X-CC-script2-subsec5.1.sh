#!/bin/bash
#SBATCH --cpus-per-task=8
#SBATCH --mem=12GB
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --gres=gpu:1


module load python
source ~/env/bin/activate

sleep 5
seed=1

time=${timeReduced}
echo ${time}
arr_nbGranular=(5 10 15 20 30 50 100)
for nbGranular in "${arr_nbGranular[@]}"
do
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.1/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.1/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
done
wait


echo "FIM"
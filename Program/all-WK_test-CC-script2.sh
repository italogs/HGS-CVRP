#!/bin/bash
#SBATCH --cpus-per-task=6
#SBATCH --mem=12GB
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --gres=gpu:1


module load python
source ~/env/bin/activate

sleep 5


seed=1

cap_instance_id=$(($instance_id + $step_instances))
echo "Instances: ${instance_id} to ${cap_instance_id}"

trained_models=(10dim-5layers-1500epochs 10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
for model in "${trained_models[@]}"
do

    for (( it = ${instance_id} ; it < ${instance_id} + 1; it++))
    do
        instance="WK_test_${it}"
        cd DPDP/
        python generate_instancePKL_each_100_customers.py ${instance}

        cd dpdp
        CUDA_VISIBLE_DEVICES=0,0 python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_${model}.json

        cd ..
        python convert_heatmapsPKL_to_readable.py ${instance}
        cd ..
    done 


    
    for (( it = ${instance_id} ; it < ${cap_instance_id}; it++))
    do
        instance="WK_test_${it}"
        cd DPDP/
        python generate_instancePKL_each_100_customers.py ${instance}

        cd dpdp
        CUDA_VISIBLE_DEVICES=0,0 python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_${model}.json

        cd ..
        python convert_heatmapsPKL_to_readable.py ${instance}
        cd ..
    done 


    for (( it = ${instance_id} ; it < ${cap_instance_id}; it++))
    do
        arr_nbGranular=(20 30 50)
        instance="WK_test_${it}"
        echo "${instance}"
        # Spawn 12 processes at once
        for nbGranular in "${arr_nbGranular[@]}"
        do
            useHeatmap=0
            crossover=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmap=1
            crossover=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
            wait

            useHeatmap=0
            crossover=9
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmap=1
            crossover=9
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
            wait
        done        
       
    done

done

for (( it = ${instance_id} ; it < ${cap_instance_id}; it++))
do
    instance="WK_test_${it}"
    rm -rf DPDP/Heatmaps_for_HGS/${instance}/
    rm -rf DPDP/dpdp/results/vrp/${instance}/
done

echo "FIM"



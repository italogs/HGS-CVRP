#!/bin/bash
#SBATCH --gres=gpu:1   
#SBATCH --cpus-per-task=32
#SBATCH --mem=64GB       
#SBATCH --time=48:00:00
#SBATCH --nodes=1


module load python
source ~/env/bin/activate

sleep 5


echo "batch_size: ${batch_size}"
trained_models=(10dim-5layers-1500epochs 10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
seed=1

for model in "${trained_models[@]}"
do
    cd DPDP
    python generate_instancePKL_each_100_customers.py ${instance}

    cd dpdp
    python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size ${batch_size} -c logs/vrp_uchoa100/config_${model}.json

    echo "Final export"

    cd ..
    python convert_heatmapsPKL_to_readable.py ${instance}
    cd ..
    echo "Convert heatmaps"
    time=${timeClassic}
    echo ${time}
    arr_nbGranular=(20 30 50)
    # Spawn 12 processes at once
    for nbGranular in "${arr_nbGranular[@]}"
    do
        echo ${nbGranular}
        useHeatmap=0
        crossover=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

        useHeatmap=1
        crossover=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

        useHeatmap=0
        crossover=9
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &            

        useHeatmap=1
        crossover=9
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    done
    
    wait



    time=${timeReduced}
    echo ${time}
    arr_nbGranular=(20 30 50)
    # Spawn 12 processes at once
    for nbGranular in "${arr_nbGranular[@]}"
    do
        echo ${nbGranular}
        useHeatmap=0
        crossover=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

        useHeatmap=1
        crossover=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

        useHeatmap=0
        crossover=9
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &            

        useHeatmap=1
        crossover=9
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmap${useHeatmap}_crossover${crossover}/${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useHeatmap ${useHeatmap} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmap${useHeatmap}_crossover${crossover}/output_${instance}_useHeatmap${useHeatmap}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
    done
    wait


    rm -rf DPDP/Heatmaps_for_HGS/${instance}
done 

echo "FIM"